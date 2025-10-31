#include "simdmonte/misc/market_data.h"
#include "simdmonte/option/option_asian.h"
#include "simdmonte/option/option_european.h"
#include "simdmonte/pricer/pricer.h"
#include "simdmonte/pricer/params.h"

#include <memory>
#include <iostream>

using namespace simdmonte;


int main() {

  // Example: Simple European option with 1 billion simulations

  MarketData market(
      100.0, // Spot
      0.05,  // Risk free rate
      0.20   // Volatility
      );
  double strike = 100.0;
  double expiry = 1.0; // Years decimal

  int n_sims = 1000000000;
  int n_steps = 1; // We can run 1 step for European options with GBM becase
                   // 1. European options are not path dependent - we only need the final price
                   // 2. For GBM, 252 1-day steps and 1 1-year step are mathematically equivalent

  Params params(n_steps, n_sims, params::UnderlyingModel::GBM, params::NormalMethod::BoxMuller);

  std::unique_ptr<Option> european_option =
    std::make_unique<EuropeanOption>(strike, expiry, EuropeanOption::OptionType::Call);

  MCPricer pricer1{params};

  double price = pricer1.price(*european_option, market);

  std::cout << price << "\n";

  // Example: An asian option with the same setup, on the same underlying, with 10 million simulations

  n_sims = 10000000;
  n_steps = 252;

  params = Params(n_steps, n_sims, params::UnderlyingModel::GBM, params::NormalMethod::BoxMuller);

  std::unique_ptr<Option> asian_option =
    std::make_unique<AsianOption>(strike, expiry, AsianOption::OptionType::Call, AsianOption::StrikeType::Fixed, 1.0f);

  MCPricer pricer2{params};

  price = pricer2.price(*asian_option, market);

  std::cout << price << "\n";

}
