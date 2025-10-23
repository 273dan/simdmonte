#include <cstdint>
#include <cstring>
#include <immintrin.h>
#include <iostream>
#include "simdmonte/misc/utils.h"


namespace simdmonte {

void debug_256(__m256 vec) {
  float array[8];

  _mm256_storeu_ps(array, vec);

  std::cout << "{ ";
  for(auto i: array) {

    std::cout << i << " ";

  }
  std::cout << "}\n";

}

float u64_to_float(uint64_t n) {
  uint32_t top_24 = static_cast<uint32_t>(n >>(64 - 24));

  // force into range 1-2
  uint32_t as_float = top_24 | 0x3F800000;

  float result;
  // should be optimised away to act like reinterpret cast
  std::memcpy(&result, &as_float, sizeof(float));
  result--;

  return result;

};

}
