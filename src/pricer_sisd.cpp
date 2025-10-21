#include "pricer_sisd.h"
#include "market_data.h"
#include <random>
#include <iostream>

using MCSISD = MCPricerSISD;
MCSISD::MCPricerSISD(int sims, int steps) : n_sims(sims), n_steps(steps) {};
double MCSISD::price(const Option& option, const MarketData& market) const {
  // Using GBM
  double dt = option.expiry / static_cast<double>(n_steps);
  double drift = (market.risk_free_rate - (0.5 * market.volatility * market.volatility)) * dt;
  double vol_dt = std::sqrt(dt) * market.volatility;
  std::vector<double> payoffs {};
  for(int i = 0; i < n_sims; i++) {
    std::vector<double> path = generate_path(market.spot, dt, drift, vol_dt);
    double payoff = option.payoff(path);
    payoffs.push_back(payoff);
    if(i % 10000 == 0) {
      std::cout << "Simulation: " << i << " / " << n_sims << "\n";
    }
  }
  double total = std::accumulate(payoffs.begin(), payoffs.end(), 0);
  double average = total / n_sims;
  double discount = average * std::exp(-market.risk_free_rate * option.expiry);
  return discount;

}

std::vector<double> MCSISD::generate_path(double spot, double dt, double drift, double vol_dt) const {
  std::vector<double> path {};
  path.push_back(spot);
  double current_price = spot;

  std::random_device rd;
  std::normal_distribution<double> dist(0.0, 1.0);
  std::mt19937 gen(rd());

  for(int i = 1; i < n_steps; i++) {
    double Z = dist(gen);
    double next_price = current_price * std::exp(drift + vol_dt * Z);
    path.push_back(next_price);
    current_price = next_price;
  }

  return path;
}

