#pragma once
#include "pricer.h"
#include "market_data.h"

class MCPricerSIMD : public IPricer {
public:
  virtual double price(const Option& option, const MarketData& market) const override;
  MCPricerSIMD(int sims, int steps);

private:
  int n_sims;
  int n_steps;

};
