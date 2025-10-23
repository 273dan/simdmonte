#include "simdmonte/pricer/pricer_sisd.h"
#include "simdmonte/misc/market_data.h"
#include <random>
#include <iostream>
namespace simdmonte {


using MCSISD = MCPricerSISD;
MCSISD::MCPricerSISD(int sims, int steps) : n_sims(sims), n_steps(steps) {};
float MCSISD::price(const Option& option, const MarketData& market) const {
  // Using GBM
  float dt = option.expiry / static_cast<float>(n_steps);
  float drift = (market.risk_free_rate - (0.5 * market.volatility * market.volatility)) * dt;
  float vol_dt = std::sqrt(dt) * market.volatility;
  float payoff_total = 0.0;
  for(int i = 0; i < n_sims; i++) {
    std::vector<float> path = generate_path(market.spot, dt, drift, vol_dt);
    float payoff = option.payoff(path);
    payoff_total += payoff;
    if(i % 10000 == 0) {
      std::cout << "Simulation: " << i << " / " << n_sims << "\n";
    }
  }
  float average = payoff_total / n_sims;
  float discount = average * std::exp(-market.risk_free_rate * option.expiry);
  return discount;

}

std::vector<float> MCSISD::generate_path(float spot, float dt, float drift, float vol_dt) const {
  std::vector<float> path {};
  path.reserve(n_steps);
  path.push_back(spot);
  float current_price = spot;

  std::random_device rd;
  std::normal_distribution<float> dist(0.0, 1.0);
  std::mt19937 gen(rd());
  for(int i = 1; i < n_steps; i++) {
      float Z = dist(gen);
      float next_price = current_price * std::exp(drift + vol_dt * Z);
      path.push_back(next_price);
      current_price = next_price;
    }

  return path;
}

}
