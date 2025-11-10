#include "simdmonte/rng/rng.h"
#include "simdmonte/rng/rng_constants.h"
#include "simdmonte/misc/utils.h"
#include "simdmonte/avx_mathfun_wrapper.h"
#include <cstring>
#include <cmath>
#include <immintrin.h>
#include <random>
#include <stdexcept>


namespace simdmonte {

Rng::Rng() {

  std::random_device rd{};

  uint64_t seeds0[4];
  uint64_t seeds1[4];

  for(int i = 0; i < 4; i++) {
    // combining two random u32s to make a u64
    // rd()s output type is 'unsigned long' which for MSVC is a 32
    seeds0[i] = rd() | (static_cast<uint64_t>(rd()) << 32);
    seeds1[i] = rd() | (static_cast<uint64_t>(rd()) << 32);

    if(seeds0[i] == 0) seeds0[i] = 1;
    if(seeds1[i] == 0) seeds1[i] = 1;
  }

  state0_ = _mm256_loadu_si256((__m256i*)seeds0);
  state1_ = _mm256_loadu_si256((__m256i*)seeds1);
}

Rng::Rng(uint64_t seed) {
  uint64_t seeds0[4] = {seed, seed+1, seed+2, seed+3};
  uint64_t seeds1[4] = {seed+4, seed+5, seed+6, seed+7};

  for(int i = 0; i < 4; i++) {
    if(seeds0[i] == 0) seeds0[i] = 1;
    if(seeds1[i] == 0) seeds1[i] = 1;
  }
  state0_ = _mm256_loadu_si256((__m256i*)seeds0);
  state1_ = _mm256_loadu_si256((__m256i*)seeds1);
}

__m256i Rng::advance_state() const {

  // implementation of xorshift128+
  // https://en.wikipedia.org/wiki/Xorshift

  __m256i s1 = state0_;
  const __m256i s0 = state1_;
  state0_ = s0;

  // ^= s1 << 23
  s1 = _mm256_xor_si256(s1, _mm256_slli_epi64(s1, 23));
  // ^= s1 >> 18
  s1 = _mm256_xor_si256(s1, _mm256_srli_epi64(s1, 18));
  // ^= s0
  s1 = _mm256_xor_si256(s1, s0);
  // ^= s0 >> 5
  s1 = _mm256_xor_si256(s1, _mm256_srli_epi64(s0, 5));

  state1_ = s1;

  // s1 + s2 (scramble)
  return _mm256_add_epi64(s1, s0);

}


__m256 Rng::uniform() const {
  __m256i rand_u64s_1 = advance_state();
  __m256i rand_u64s_2 = advance_state();

  uint64_t u64s_1[4];
  uint64_t u64s_2[4];


  _mm256_storeu_si256((__m256i*)u64s_1, rand_u64s_1);
  _mm256_storeu_si256((__m256i*)u64s_2, rand_u64s_2);


  float result[8];

  for(int i = 0; i < 4; i++) {
    result[i] = u64_to_float(u64s_1[i]);
    result[i + 4] = u64_to_float(u64s_2[i]);
  }

  __m256 floats = _mm256_loadu_ps(result);

  // we often can't use 0: check for zeroes
  __m256 zero_mask = _mm256_cmp_ps(floats, _mm256_setzero_ps(), _CMP_EQ_OQ);

  const __m256 float_mins = _mm256_set1_ps(std::numeric_limits<float>::min());

  floats = _mm256_blendv_ps(floats, float_mins, zero_mask);

  return floats;


}

__m256 Rng::uniform_simd() const {
  __m256i rand_u64s_1 = advance_state();
  __m256i rand_u64s_2 = advance_state();

  __m256i shifted_1 = _mm256_srli_epi64(rand_u64s_1, 40);
  __m256i shifted_2 = _mm256_srli_epi64(rand_u64s_2, 40);


  const __m256i permutation_mask = _mm256_setr_epi32(0, 2, 4, 6, 0, 0, 0, 0);
  __m256i permuted_1 = _mm256_permutevar8x32_epi32(shifted_1,  permutation_mask);
  __m256i permuted_2 = _mm256_permutevar8x32_epi32(shifted_2,  permutation_mask);


  __m128i narrowed_1 = _mm256_castsi256_si128(permuted_1);
  __m128i narrowed_2 = _mm256_castsi256_si128(permuted_2);

  const __m128i mask_float_bits_for_int = _mm_set1_epi32(0x3F800000);

  __m128i float_bits_1 = _mm_or_si128(narrowed_1, mask_float_bits_for_int);
  __m128i float_bits_2 = _mm_or_si128(narrowed_2, mask_float_bits_for_int);

  __m128 as_float_1 = _mm_castsi128_ps(float_bits_1);
  __m128 as_float_2 = _mm_castsi128_ps(float_bits_2);


  __m256 combined_floats = _mm256_castps128_ps256(as_float_1);
  combined_floats = _mm256_insertf128_ps(combined_floats, as_float_2, 1);

  const __m256 ones = _mm256_set1_ps(1.0f);

  __m256 floats_simd = _mm256_sub_ps(combined_floats, ones);


  uint64_t u64s_1[4];
  uint64_t u64s_2[4];

  // we often can't use 0: check for zeroes
  __m256 zero_mask = _mm256_cmp_ps(floats_simd, _mm256_setzero_ps(), _CMP_EQ_OQ);

  const __m256 float_mins = _mm256_set1_ps(std::numeric_limits<float>::min());

  floats_simd = _mm256_blendv_ps(floats_simd, float_mins, zero_mask);

  return floats_simd;


}

__m256 Rng::normal(const params::NormalMethod method) const {
  switch(method) {
    case(params::NormalMethod::BoxMuller) : return box_muller_transform();
    case(params::NormalMethod::InverseCDF) : return inverse_cdf_approx();
  }
  throw std::invalid_argument("Unknown normal method");
}

__m256 Rng::box_muller_transform() const {
  if(bm_cached) {
    bm_cached = false;
    return bm_cached_values;
  }


  // implementation of Box-Muller transform
  // https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform

  __m256 U1 = uniform_simd();
  __m256 U2 = uniform_simd();

  __m256 pi = _mm256_set1_ps(M_PI);

  // sqrt(-2ln(U1))
  __m256 R = _mm256_sqrt_ps(
      _mm256_mul_ps(
        _mm256_set1_ps(-2.0), log256_ps(U1)
      )
    );

  // 2pi * U2
  __m256 Theta = _mm256_mul_ps(
      _mm256_mul_ps(
        _mm256_set1_ps(2), pi
      ),
      U2
    );

  // R * cos(Theta)
  __m256 Z0 = _mm256_mul_ps(
      R, cos256_ps(Theta)
      );
  // R * sin(Theta)
  __m256 Z1 = _mm256_mul_ps(
      R, sin256_ps(Theta)
      );

  bm_cached_values = Z1;
  bm_cached = true;
  return Z0;

}


__m256 Rng::inverse_cdf_approx() const {
  
  // implementation of Peter Acklam's algorithm
  // https://stackedboxes.org/2017/05/01/acklams-normal-quantile-function/

  __m256 p = uniform();

  __m256 mask_low = _mm256_cmp_ps(p, constants::P_LOWS, _CMP_LT_OQ);
  __m256 mask_high = _mm256_cmp_ps(p, constants::P_HIGHS, _CMP_GT_OQ);


  __m256 q = _mm256_sub_ps(p, constants::HALF);
  __m256 r = _mm256_mul_ps(q, q);

  // center
  __m256 numerator = _mm256_fmadd_ps(constants::A1, r, constants::A2);
  numerator = _mm256_fmadd_ps(numerator, r, constants::A3);
  numerator = _mm256_fmadd_ps(numerator, r, constants::A4);
  numerator = _mm256_fmadd_ps(numerator, r, constants::A5);
  numerator = _mm256_fmadd_ps(numerator, r, constants::A6);
  numerator = _mm256_mul_ps(numerator, q);

  __m256 denominator = _mm256_fmadd_ps(constants::B1, r, constants::B2); 
  denominator = _mm256_fmadd_ps(denominator, r, constants::B3);
  denominator = _mm256_fmadd_ps(denominator, r, constants::B4);
  denominator = _mm256_fmadd_ps(denominator, r, constants::B5);
  denominator = _mm256_fmadd_ps(denominator, r, constants::ONES);

  __m256 center = _mm256_div_ps(numerator, denominator);

  // tails
  __m256 q_low = _mm256_sqrt_ps(
      _mm256_mul_ps(constants::NEGTWOS, log256_ps(p))
      );
  __m256 one_minus_p = _mm256_sub_ps(constants::ONES, p);
  __m256 q_high = _mm256_sqrt_ps(
      _mm256_mul_ps(constants::NEGTWOS, log256_ps(one_minus_p))
      );

  __m256 q_tail = _mm256_blendv_ps(q_low, q_high, mask_high);
  __m256 numerator_tail = _mm256_fmadd_ps(constants::C1, q_tail, constants::C2);
  numerator_tail = _mm256_fmadd_ps(numerator_tail, q_tail, constants::C3);
  numerator_tail = _mm256_fmadd_ps(numerator_tail, q_tail, constants::C4);
  numerator_tail = _mm256_fmadd_ps(numerator_tail, q_tail, constants::C5);
  numerator_tail = _mm256_fmadd_ps(numerator_tail, q_tail, constants::C6);

  __m256 denominator_tail = _mm256_fmadd_ps(constants::D1, q_tail, constants::D2); 
  denominator_tail = _mm256_fmadd_ps(denominator_tail, q_tail, constants::D3);
  denominator_tail = _mm256_fmadd_ps(denominator_tail, q_tail, constants::D4);
  denominator_tail = _mm256_fmadd_ps(denominator_tail, q_tail, constants::ONES);

  __m256 low = _mm256_div_ps(numerator_tail, denominator_tail);
  __m256 high = _mm256_sub_ps(constants::ZEROES, low);

  // recombine
  __m256 z = center;
  z = _mm256_blendv_ps(z, low, mask_low);
  z = _mm256_blendv_ps(z, high, mask_high);

  return z;
}
} // namespace simdmonte
