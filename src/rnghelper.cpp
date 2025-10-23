#include "simdmonte/rnghelper/rnghelper.h"
#include <cstring>
#include <cmath>


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

float RngHelper::u64_to_float(uint64_t n) {
  uint32_t top_24 = static_cast<uint32_t>(n >>(64 - 24));

  // force into range 1-2
  uint32_t as_float = top_24 | 0x3F800000;

  float result;
  // should be optimised away to act like reinterpret cast
  std::memcpy(&result, &as_float, sizeof(float));

  return result;
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

  return _mm256_loadu_ps(result);
}

__m256 RngHelper::normal_floats_8() const {




}




