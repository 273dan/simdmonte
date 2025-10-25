#include <benchmark/benchmark.h>
#include "simdmonte/misc/market_data.h"
#include "simdmonte/option/option_european.h"
#include "simdmonte/pricer/pricer_simd.h"
#include "simdmonte/pricer/pricer_sisd.h"

#include <memory>
#include <iostream>


using namespace simdmonte;
static void BM_EuropeanOption_SISD(benchmark::State& state) {

  MarketData market(
      100.0, // Spot
      0.05,  // Risk free rate
      0.20   // Volatility
      );
  double strike = 100.0;
  double expiry = 1.0; // Years decimal

  int n_sims = 1000;
  int n_steps = 252;
  std::unique_ptr<Option> option =
    std::make_unique<EuropeanOption>(strike, expiry, EuropeanOption::OptionType::Call);

  std::unique_ptr<IPricer> pricer = 
    std::make_unique<MCPricerSISD>(n_sims, n_steps);
  for(auto _: state) {
    double price = pricer->price(*option, market);
  }

}
BENCHMARK(BM_EuropeanOption_SISD);

static void BM_EuropeanOption_SIMD(benchmark::State& state) {

  simdmonte::MarketData market(
      100.0, // Spot
      0.05,  // Risk free rate
      0.20   // Volatility
      );
  double strike = 100.0;
  double expiry = 1.0; // Years decimal

  int n_sims = 1000;
  int n_steps = 252;
  std::unique_ptr<Option> option =
    std::make_unique<EuropeanOption>(strike, expiry, EuropeanOption::OptionType::Call);

  std::unique_ptr<IPricer> pricer = 
    std::make_unique<MCPricerSIMD>(n_sims, n_steps);
  for(auto _: state) {
    double price = pricer->price(*option, market);
  }

}
BENCHMARK(BM_EuropeanOption_SIMD);
