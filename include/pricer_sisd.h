#pragma once
#include "market_data.h"
#include "pricer.h"

class MCPricerSISD : public IPricer {
public:
  virtual double price(const Option& option, const MarketData& market) const override;
  MCPricerSISD(int sims, int steps);

private:
  int n_sims;
  int n_steps;
  std::vector<double> generate_path(double spot, double dt, double drift, double vol_dt) const;

};
