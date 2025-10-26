#pragma once
#include "simdmonte/option/option.h"
#include "simdmonte/pricer/params.h"
#include "simdmonte/rng/rng.h"
#include "simdmonte/underlying/underlying.h"
#include "simdmonte/misc/market_data.h"
#include <immintrin.h>
namespace simdmonte {

class GBMLogUnderlying : public IUnderlying {
public:
  virtual __m256 step() override;
  GBMLogUnderlying(const Option& option, const MarketData& market_data,const Params& params);
  void set_current(float prices) override;
  __m256 get_special() override;
  __m256 get_non_log_prices();
private:
  const float dt_;
  mutable Rng rng_;
  mutable __m256 current_;
  const __m256 drifts_;
  const __m256 vol_dts_;
  const params::NormalMethod normal_method_;

};


} // namespace simdmonte
