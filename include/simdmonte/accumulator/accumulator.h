#pragma once
#include <immintrin.h>

#include "simdmonte/misc/price_types.h"
namespace simdmonte {

class IAccumulator {
 public:
  virtual ~IAccumulator() = 0;
  virtual void update(const LogSpaceVec& prices) = 0;
  virtual void update(const PriceSpaceVec& prices) = 0;
  virtual __m256 payoffs() = 0;
  virtual void reset() = 0;
};
}  // namespace simdmonte
