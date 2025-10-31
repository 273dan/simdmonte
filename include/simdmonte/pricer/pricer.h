#pragma once
#include "simdmonte/misc/market_data.h"
#include "simdmonte/pricer/params.h"
#include "simdmonte/option/option.h"

namespace simdmonte {


class MCPricer {
public:
  float price(const Option& option, const MarketData& market) const;
  MCPricer(Params params_);
  MCPricer() = default;

private:
  int n_sims_;
  int n_steps_;
  Params params_;


};
}
