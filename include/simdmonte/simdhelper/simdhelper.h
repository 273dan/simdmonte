#pragma once
#include <immintrin.h>
namespace simdmonte {


class ISimdHelper {
public:
  virtual ~ISimdHelper() = 0;
  virtual void update(const __m256& prices) = 0;
  virtual __m256 payoffs() = 0;

};

struct SimdHelperParams {
  bool use_log_prices = false;
};
}
