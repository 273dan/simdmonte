#include "simdmonte/underlying/underlying_gbmlog.h"
#include "simdmonte/rng/rng.h"
#include <cmath>
#include <immintrin.h>
namespace simdmonte {


GBMLogUnderlying::GBMLogUnderlying(const Option& option, const MarketData& market, const Params& params) :
  dt_(option.expiry / static_cast<float>(params.n_steps)),
  rng_(params.seed == 0 ? Rng() : Rng(params.seed)),
  current_(_mm256_set1_ps(std::log(market.spot))),
  drifts_(_mm256_set1_ps((market.risk_free_rate - (0.5 * market.volatility * market.volatility)) * dt_)),
  vol_dts_(_mm256_set1_ps(std::sqrt(dt_) * market.volatility)),
  normal_method_(params.normal_method) {};




LogSpaceVec GBMLogUnderlying::step() {
  __m256 Z = rng_.normal(normal_method_);
  __m256 shocks = _mm256_mul_ps(vol_dts_, Z);
  current_ = _mm256_add_ps(current_, shocks);
  current_ = _mm256_add_ps(current_, drifts_);
  return LogSpaceVec{current_};
}

void GBMLogUnderlying::set_current(float prices) {
  current_ = _mm256_set1_ps(prices);
}







}
