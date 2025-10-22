#pragma once
#include "simdmonte/pricer/pricer.h"
#include "simdmonte/misc/market_data.h"
#include <random>

class MCPricerSIMD : public IPricer {
public:
  virtual float price(const Option& option, const MarketData& market) const override;
  MCPricerSIMD(int sims, int steps);

private:
  int n_sims;
  int n_steps;
  __m256 packed_float_normals() const;
  mutable std::normal_distribution<float> dist_;
  mutable std::mt19937 gen_;

};
