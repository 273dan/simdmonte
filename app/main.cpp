#include "simdmonte/misc/market_data.h"
#include "simdmonte/option/option_european.h"
#include "simdmonte/pricer/pricer_simd.h"
#include "simdmonte/pricer/pricer_sisd.h"

#include <memory>
#include <iostream>

int main() {
  MarketData market(
      100.0, // Spot
      0.05,  // Risk free rate
      0.20   // Volatility
      );
  double strike = 100.0;
  double expiry = 1.0; // Years decimal

  int n_sims = 100000;
  int n_steps = 252;
  std::unique_ptr<Option> option =
    std::make_unique<EuropeanOption>(strike, expiry, EuropeanOption::OptionType::Call);

  std::unique_ptr<IPricer> pricer = 
    std::make_unique<MCPricerSIMD>(n_sims, n_steps);

  double price = pricer->price(*option, market);

  std::cout << price << "\n";

}
