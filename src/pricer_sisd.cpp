#include "simdmonte/pricer/pricer_sisd.h"
#include "simdmonte/option/option_asian.h"
#include "simdmonte/option/option_european.h"
#include "simdmonte/pricer/params.h"
#include <cmath>
#include <random>

namespace simdmonte {

MCPricerSISD::MCPricerSISD(Params params) : n_sims_(params.n_sims), n_steps_(params.n_steps), params_(params) {}

float MCPricerSISD::price(const EuropeanOption& option, const MarketData& market) const {
  // We will use GBM for this implementation, so n_steps is 1
  double sum = 0.0;
  float dt = option.expiry;
  float vol_dt = market.volatility * std::sqrt(option.expiry);
  float drift = (market.risk_free_rate - (0.5 * market.volatility * market.volatility)) * option.expiry;
  std::mt19937 gen(std::random_device{}());
  std::normal_distribution<float> dist{0.0f, 1.0f};

  for(int i = 0; i < n_sims_; i++) {
    float Z = dist(gen);
    float price = market.spot * std::exp(drift + vol_dt * Z);
    float payoff;
    if(option.type == EuropeanOption::OptionType::Call) {
      payoff = std::max(price - option.strike, 0.0f);
    }
    else {
      payoff = std::max(option.strike - price, 0.0f);
    }

    sum += payoff;
  }

  float average = sum / static_cast<float>(n_sims_);
  return average * std::exp(-market.risk_free_rate * option.expiry);
}
float MCPricerSISD::price(const AsianOption& option, const MarketData& market) const {
  double sum = 0.0;
  float dt = option.expiry / static_cast<float>(n_steps_);
  float vol_dt = market.volatility * std::sqrt(option.expiry);
  float drift = (market.risk_free_rate - (0.5 * market.volatility * market.volatility)) * option.expiry;
  float payoff_total{0.0f};
  std::mt19937 gen(std::random_device{}());
  std::normal_distribution<float> dist{0.0f, 1.0f};
  
  int j_priced{0};
  int pricing_start = static_cast<int>(((option.expiry - option.avg_period) / option.expiry) * n_steps_);
  for(int i = 0; i < n_sims_; i++) {
    float current{market.spot};
    float price_total{0.0f};
    for(int j = 0; j < n_steps_; j++) {
    float Z = dist(gen);
    current = current * std::exp(drift + vol_dt * Z);

      if(j > pricing_start) {
        j_priced++;
        price_total += current;
      }
    }
    float avg = price_total / j_priced;
    float lhs, rhs;
    float payoff;
    if (option.strike_type == AsianOption::StrikeType::Fixed) {
      lhs = avg;
      rhs = option.strike;
    }
    else {
      lhs = current;
      rhs = avg;
    }

    if (option.option_type == AsianOption::OptionType::Call) {
      payoff = lhs - rhs;
    }
    else {
      payoff = rhs - lhs;
    }

    payoff_total += std::max(0.0f, payoff);
  }
  float average_payoff{payoff_total / n_sims_};

  return average_payoff * std::exp(-market.risk_free_rate * option.expiry);
}
}
