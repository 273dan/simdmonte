#include "simdmonte/pricer/pricer_sisd.h"
#include "simdmonte/option/option_european.h"
#include "simdmonte/pricer/params.h"
#include <cmath>
#include <random>

namespace simdmonte {

MCPricerSISD::MCPricerSISD(Params params) : n_sims_(params.n_sims), n_steps_(params.n_steps), params_(params) {}

float MCPricerSISD::price(const EuropeanOption& euro_option, const MarketData& market) const {
  // We will use GBM for this implementation, so n_steps is 1
  double sum = 0.0;
  float dt = euro_option.expiry;
  float vol_dt = market.volatility * std::sqrt(euro_option.expiry);
  float drift = (market.risk_free_rate - (0.5 * market.volatility * market.volatility)) * euro_option.expiry;
  std::mt19937 gen(std::random_device{}());
  std::normal_distribution<float> dist{0.0f, 1.0f};

  for(int i = 0; i < n_sims_; i++) {
    float Z = dist(gen);
    float price = market.spot * std::exp(drift + vol_dt * Z);
    float payoff;
    if(euro_option.type == EuropeanOption::OptionType::Call) {
      payoff = std::max(price - euro_option.strike, 0.0f);
    }
    else {
      payoff = std::max(euro_option.strike - price, 0.0f);
    }

    sum += payoff;
  }

  float average = sum / static_cast<float>(n_sims_);
  return average * std::exp(-market.risk_free_rate * euro_option.expiry);
}
}
