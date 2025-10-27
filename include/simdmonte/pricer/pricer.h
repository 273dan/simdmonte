#pragma once
#include "simdmonte/option/option.h"
#include "simdmonte/misc/market_data.h"
namespace simdmonte {

class IPricer {
public:
  virtual ~IPricer() = 0;
  virtual float price(const Option& option, const MarketData& market) const = 0;
  virtual float price_concurrent(const Option& option, const MarketData& market) const = 0;
};

}
