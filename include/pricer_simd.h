#pragma once
#include "pricer.h"
#include "market_data.h"
#include <random>

class MCPricerSIMD : public IPricer {
public:
  virtual double price(const Option& option, const MarketData& market) const override;
  MCPricerSIMD(int sims, int steps);

private:
  int n_sims;
  int n_steps;
  __m256d packed_double_normals() const;
  mutable std::normal_distribution<double> dist_;
  mutable std::mt19937 gen_;

};
