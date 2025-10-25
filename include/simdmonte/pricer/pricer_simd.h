#pragma once
#include "simdmonte/pricer/pricer.h"
#include "simdmonte/misc/market_data.h"
#include <random>

namespace simdmonte {


class MCPricerSIMD : public IPricer {
public:
  virtual float price(const Option& option, const MarketData& market) const override;
  MCPricerSIMD(int sims, int steps);

private:
  int n_sims;
  int n_steps;

};
}
