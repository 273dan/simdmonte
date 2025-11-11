#pragma once
#include "simdmonte/misc/market_data.h"
#include "simdmonte/pricer/params.h"
#include "simdmonte/option/option.h"

namespace simdmonte {


class Pricer {
public:
  float price(const Option& option, const MarketData& market) const;
  Pricer(Params params_);
  Pricer() = default;

private:
  long n_sims_;
  int n_steps_;
  Params params_;


};
}
