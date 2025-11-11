#include "simdmonte/misc/market_data.h"
#include "simdmonte/option/option_asian.h"
#include "simdmonte/pricer/pricer.h"
#include "simdmonte/pricer/params.h"

#include <memory>
#include <iostream>

using namespace simdmonte;


int main() {

  // Example: Asian option with 10 million simulations

  MarketData market{
      100.0, // The underlying is currently priced at 100 spot
      0.05,  // The risk free rate is 5%
      0.20   // The underlying has a volatility of 20%
  };

  Params params{

    /* n_steps */
    252, /* For a path dependent option we can't take a shortcut by using 1 step */

    /* n_sims */
    10'000'000, 

    /* underlying_model */
    params::UnderlyingModel::GBM, /* We'd like to use Geometric Brownian Motion as the underlying model */

    /* normal_method */
    params::NormalMethod::BoxMuller /* ... and Box-Muller transform for random number generation */

  };

  Pricer pricer{params};

  // Our Asian option...
  float strike = 100.0f;   // has a strike of 100,
  float expiry = 1.0f;     // an expiry of 1 year,
  float avg_period = 1.0f; // and is averaged over its entire duration

  auto option_type = AsianOption::OptionType::Call;  // It is a call option,
  auto strike_type = AsianOption::StrikeType::Fixed; // with a fixed strike

  std::unique_ptr<Option> asian_option =
    std::make_unique<AsianOption>(strike, expiry, option_type, strike_type, avg_period);

  float price = pricer.price(*asian_option, market);

  std::cout << price << "\n";

  










}
