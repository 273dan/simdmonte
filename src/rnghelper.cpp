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


}
