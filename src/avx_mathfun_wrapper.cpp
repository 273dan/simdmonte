#include "simdmonte/avx_mathfun_wrapper.h"
extern "C" {
#include "avx_mathfun/avx_mathfun.h"
}
namespace simdmonte {
__m256 cos256_ps(__m256 x) { return ::cos256_ps(x); }
__m256 sin256_ps(__m256 x) { return ::sin256_ps(x); }
__m256 exp256_ps(__m256 x) { return ::exp256_ps(x); }
__m256 log256_ps(__m256 x) { return ::log256_ps(x); }
}  // namespace simdmonte
