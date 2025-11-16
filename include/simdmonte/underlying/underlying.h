#pragma once
#include <immintrin.h>

#include "simdmonte/misc/price_types.h"
namespace simdmonte {
class IUnderlying {
 public:
  virtual ~IUnderlying() = 0;
  virtual LogSpaceVec step() = 0;
  virtual void set_current(float price) = 0;
};

}  // namespace simdmonte
