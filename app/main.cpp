#include "market_data.h"
#include "option_european.h"
#include "pricer_simd.h"
#include "pricer_sisd.h"

#include <memory>
#include <iostream>

int main() {
  MarketData market(100.0, 0.05, 0.20);
  double strike = 100.0;
  double expiry = 1.0;

  int n_sims = 1000000;
  int n_steps = 252;

  std::unique_ptr<Option> option =
    std::make_unique<EuropeanOption>(strike, expiry, EuropeanOption::OptionType::Call);

  std::unique_ptr<IPricer> pricer = 
    std::make_unique<MCPricerSIMD>(n_sims, n_steps);

  double price = pricer->price(*option, market);

  std::cout << price << "\n";

}
