#include "simdmonte/rnghelper/rnghelper.h"
#include "simdmonte/misc/utils.h"
#include "avx_mathfun/avx_mathfun.h"
#include <cstring>
#include <cmath>
#include <random>


namespace simdmonte {

RngHelper::RngHelper() {

  std::random_device rd{};

  uint64_t seeds0[4];
  uint64_t seeds1[4];

  for(int i = 0; i < 4; i++) {
    // Combining two random u32s to make a u64
    // rd()s output type is 'unsigned long' which for MSVC is a 32
    seeds0[i] = rd() | (static_cast<uint64_t>(rd()) << 32);
    seeds1[i] = rd() | (static_cast<uint64_t>(rd()) << 32);

    if(seeds0[i] == 0) seeds0[i] = 1;
    if(seeds1[i] == 0) seeds1[i] = 1;
  }

  state0_ = _mm256_loadu_si256((__m256i*)seeds0);
  state1_ = _mm256_loadu_si256((__m256i*)seeds1);
}

RngHelper::RngHelper(uint64_t seed) {
  uint64_t seeds0[4] = {seed, seed+1, seed+2, seed+3};
  uint64_t seeds1[4] = {seed+4, seed+5, seed+6, seed+7};

  for(int i = 0; i < 4; i++) {
    if(seeds0[i] == 0) seeds0[i] = 1;
    if(seeds1[i] == 0) seeds1[i] = 1;
  }
  state0_ = _mm256_loadu_si256((__m256i*)seeds0);
  state1_ = _mm256_loadu_si256((__m256i*)seeds1);
}

__m256i RngHelper::advance_state() const {

  // Implementation of xorshift128+
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


__m256 RngHelper::unif_floats_8() const {
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

  __m256 floats_simd = _mm256_loadu_ps(result);

  // we often can't use 0: check for zeroes
  __m256 zero_mask = _mm256_cmp_ps(floats_simd, _mm256_setzero_ps(), _CMP_EQ_OQ);

  const __m256 float_mins = _mm256_set1_ps(std::numeric_limits<float>::min());

  floats_simd = _mm256_blendv_ps(floats_simd, float_mins, zero_mask);

  return floats_simd;


}

__m256 RngHelper::normal_floats_8() const {
  if(cached) {
    cached = false;
    return cached_normals;
  }


  // implementation of Box-Muller transform
  // https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform

  __m256 U1 = unif_floats_8();
  __m256 U2 = unif_floats_8();

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

  cached_normals = Z1;
  cached = true;
  return Z0;

}


}
