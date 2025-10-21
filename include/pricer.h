#pragma once
#include "option.h"
#include "market_data.h"
class IPricer {
public:
  virtual ~IPricer() = 0;
  virtual double price(const Option& option, const MarketData& market) const = 0;
};


