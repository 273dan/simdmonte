#pragma once
#include "simdmonte/misc/market_data.h"
#include "simdmonte/option/option_asian.h"
#include "simdmonte/option/option_european.h"
#include "simdmonte/pricer/params.h"
namespace simdmonte {
class PricerSISD {
public:
  float price(const EuropeanOption &euro_option,
              const MarketData &market) const;
  float price(const AsianOption &asian_option, const MarketData &market) const;
  PricerSISD(Params params_);
  PricerSISD() {};

private:
  int n_sims_;
  int n_steps_;
  Params params_;
};
} // namespace simdmonte
