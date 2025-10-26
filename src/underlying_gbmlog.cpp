#include "simdmonte/underlying/underlying_gbmlog.h"
#include "simdmonte/rng/rng.h"
#include "simdmonte/avx_mathfun_wrapper.h"
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




__m256 GBMLogUnderlying::step() {
  __m256 Z = rng_.normal(Rng::NormalMethod::InverseCDF);
  __m256 shocks = _mm256_mul_ps(vol_dts_, Z);
  current_ = _mm256_add_ps(current_, shocks);
  current_ = _mm256_add_ps(current_, drifts_);
  return current_;
}

void GBMLogUnderlying::set_current(float prices) {
  current_ = _mm256_set1_ps(prices);
}

__m256 GBMLogUnderlying::get_special() {
  return exp256_ps(current_);
}






}
