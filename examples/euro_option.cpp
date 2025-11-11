#include "simdmonte/misc/market_data.h"
#include "simdmonte/option/option_european.h"
#include "simdmonte/pricer/pricer.h"
#include "simdmonte/pricer/params.h"

#include <memory>
#include <iostream>

using namespace simdmonte;


int main() {

  // Example: Simple European option with 1 billion simulations

  MarketData market{
      100.0, // The underlying is currently priced at 100 spot
      0.05,  // The risk free rate is 5%
      0.20   // The underlying has a volatility of 20%
  };

  Params params{

    /* n_steps */
    1, /* For a path independent option we can take a shortcut by jumping to expiry in 1 step */

    /* n_sims */
    1'000'000'000, 

    /* underlying_model */
    params::UnderlyingModel::GBM, /* We'd like to use Geometric Brownian Motion as the underlying model */

    /* normal_method */
    params::NormalMethod::BoxMuller /* ... and Box-Muller transform for random number generation */

  };

  Pricer pricer{params};

  // Our European option...
  float strike = 100.0f;                               // has a strike of 100,
  float expiry = 1.0f;                                 // an expiry of 1 year,
  auto option_type = EuropeanOption::OptionType::Call; // and is a call option

  std::unique_ptr<Option> european_option =
    std::make_unique<EuropeanOption>(strike, expiry, option_type);


  double price = pricer.price(*european_option, market);

  std::cout << price << "\n";

}
