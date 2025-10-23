#pragma once
#include "simdmonte/misc/market_data.h"
#include "simdmonte/pricer/pricer.h"
namespace simdmonte {



class MCPricerSISD : public IPricer {
public:
  virtual float price(const Option& option, const MarketData& market) const override;
  MCPricerSISD(int sims, int steps);

private:
  int n_sims;
  int n_steps;
  std::vector<float> generate_path(float spot, float dt, float drift, float vol_dt) const;

};
}
