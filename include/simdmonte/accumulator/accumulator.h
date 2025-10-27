#pragma once
#include <immintrin.h>
namespace simdmonte {


class IAccumulator {
public:
  virtual ~IAccumulator() = 0;
  virtual void update(const __m256& prices) = 0;
  virtual __m256 payoffs() = 0;

};
}
