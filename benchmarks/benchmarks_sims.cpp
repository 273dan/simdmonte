#include <benchmark/benchmark.h>
#include "simdmonte/option/option_european.h"
#include "simdmonte/pricer/params.h"
#include "simdmonte/pricer/pricer.h"
#include "simdmonte/pricer/pricer_simd.h"
using namespace simdmonte;

static void BM_10MSimsSIMD(benchmark::State& state) {
  MarketData market(
      100.0, // Spot
      0.05,  // Risk free rate
      0.20   // Volatility
      );


  int n_steps = 252;
  int n_sims = 10000000;
  double strike = 100.0;
  double expiry = 1.0; // Years decimal

  Params params(n_steps, n_sims, params::UnderlyingModel::GBM, params::NormalMethod::BoxMuller);

  std::unique_ptr<Option> option =
    std::make_unique<EuropeanOption>(strike, expiry, EuropeanOption::OptionType::Call);

  std::unique_ptr<IPricer> pricer = 
    std::make_unique<MCPricerSIMD>(params);

  for(auto _ : state) {
    double price = pricer->price(*option, market);
    benchmark::DoNotOptimize(price);
    state.SetItemsProcessed(n_sims);
  }


}
BENCHMARK(BM_10MSimsSIMD)->UseRealTime();
static void BM_10MSimsSIMDConcurrent(benchmark::State& state) {
  MarketData market(
      100.0, // Spot
      0.05,  // Risk free rate
      0.20   // Volatility
      );


  int n_steps = 252;
  int n_sims = 10000000;
  double strike = 100.0;
  double expiry = 1.0; // Years decimal

  Params params(n_steps, n_sims, params::UnderlyingModel::GBM, params::NormalMethod::BoxMuller);

  std::unique_ptr<Option> option =
    std::make_unique<EuropeanOption>(strike, expiry, EuropeanOption::OptionType::Call);

  std::unique_ptr<IPricer> pricer = 
    std::make_unique<MCPricerSIMD>(params);

  for(auto _ : state) {
    double price = pricer->price_concurrent(*option, market);
    benchmark::DoNotOptimize(price);
    state.SetItemsProcessed(n_sims);
  }

}
BENCHMARK(BM_10MSimsSIMDConcurrent)->UseRealTime();
static void BM_100MSimsSIMDConcurrent(benchmark::State& state) {
  MarketData market(
      100.0, // Spot
      0.05,  // Risk free rate
      0.20   // Volatility
      );


  int n_steps = 252;
  int n_sims = 100000000;
  double strike = 100.0;
  double expiry = 1.0; // Years decimal

  Params params(n_steps, n_sims, params::UnderlyingModel::GBM, params::NormalMethod::BoxMuller);

  std::unique_ptr<Option> option =
    std::make_unique<EuropeanOption>(strike, expiry, EuropeanOption::OptionType::Call);

  std::unique_ptr<IPricer> pricer = 
    std::make_unique<MCPricerSIMD>(params);

  for(auto _ : state) {
    double price = pricer->price_concurrent(*option, market);
    benchmark::DoNotOptimize(price);
    state.SetItemsProcessed(n_sims);
  }

}
BENCHMARK(BM_100MSimsSIMDConcurrent)->UseRealTime();
