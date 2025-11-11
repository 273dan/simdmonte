#pragma once
#include <immintrin.h>
namespace simdmonte {
__m256 cos256_ps(__m256 x);
__m256 sin256_ps(__m256 x);
__m256 exp256_ps(__m256 x);
__m256 log256_ps(__m256 x);
} // namespace simdmonte
