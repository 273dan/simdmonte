#pragma once
#include "simdmonte/pricer/pricer.h"
#include "simdmonte/misc/market_data.h"
#include "simdmonte/pricer/params.h"

namespace simdmonte {


class MCPricerSIMD : public IPricer {
public:
  virtual float price(const Option& option, const MarketData& market) const override;
  MCPricerSIMD(Params params_);

private:
  int n_sims_;
  int n_steps_;
  Params params_;


};
}
