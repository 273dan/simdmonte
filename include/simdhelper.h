#pragma once
#include <immintrin.h>

class ISimdHelper {
public:
  virtual ~ISimdHelper() = 0;
  virtual void update(const __m256d& prices) = 0;
  virtual __m256d payoffs() = 0;

};

struct SimdHelperParams {
  bool use_log_prices = false;
};
