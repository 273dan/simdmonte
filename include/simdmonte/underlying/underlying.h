#pragma once
#include <immintrin.h>
namespace simdmonte {
class IUnderlying {
public:
  virtual ~IUnderlying() = 0;
  virtual __m256 step() = 0;
  virtual void set_current(float price) = 0;
  virtual __m256 get_special() = 0;
};

} // namespace simdmonte
