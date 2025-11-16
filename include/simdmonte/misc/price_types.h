#pragma once
#include <immintrin.h>

#include "simdmonte/avx_mathfun_wrapper.h"
namespace simdmonte {
struct LogSpaceVec {
  __m256 value;
};

struct PriceSpaceVec {
  __m256 value;
};

inline PriceSpaceVec to_price_space(LogSpaceVec l) {
  return PriceSpaceVec{exp256_ps(l.value)};
}
inline LogSpaceVec to_log_space(PriceSpaceVec p) {
  return LogSpaceVec{log256_ps(p.value)};
}
}  // namespace simdmonte
