#include "pricer_simd.h"
#include <cmath>
#include <immintrin.h>



MCPricerSIMD::MCPricerSIMD(int sims, int steps) : n_sims(sims), n_steps(steps) {};


double MCPricerSIMD::price(const Option& option, const MarketData& market) const {
  double dt = option.expiry / static_cast<double>(n_steps);
  double drift = (market.risk_free_rate - (0.5 * market.volatility * market.volatility)) * dt;
  double vol_dt = std::sqrt(dt) * market.volatility;




}
