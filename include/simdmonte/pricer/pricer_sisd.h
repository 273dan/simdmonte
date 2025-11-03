#pragma once
#include "simdmonte/pricer/params.h"
#include "simdmonte/option/option_european.h"
#include "simdmonte/option/option_asian.h"
#include "simdmonte/misc/market_data.h"
namespace simdmonte {
class MCPricerSISD {
public:
  float price(const EuropeanOption& euro_option, const MarketData& market) const;
  float price(const AsianOption& asian_option, const MarketData& market) const;
  MCPricerSISD(Params params_);
  MCPricerSISD() {};
private:
  int n_sims_;
  int n_steps_;
  Params params_;
};
} // namespace simdmonte
