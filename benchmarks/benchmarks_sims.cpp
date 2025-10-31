#include <benchmark/benchmark.h>
#include "simdmonte/option/option_asian.h"
#include "simdmonte/option/option_european.h"
#include "simdmonte/pricer/params.h"
#include "simdmonte/pricer/pricer.h"
using namespace simdmonte;


static void BM_100MSimsSIMDConcurrentEuropean1Step(benchmark::State& state) {
  MarketData market(
      100.0, // Spot
      0.05,  // Risk free rate
      0.20   // Volatility
      );


  int n_steps = 1;
  int n_sims = 100000000;
  double strike = 100.0;
  double expiry = 1.0; // Years decimal

  Params params(n_steps, n_sims, params::UnderlyingModel::GBM, params::NormalMethod::BoxMuller);

  std::unique_ptr<Option> option =
    std::make_unique<EuropeanOption>(strike, expiry, EuropeanOption::OptionType::Call);

  MCPricer pricer{params};


  for(auto _ : state) {
    double price = pricer.price(*option, market);
    benchmark::DoNotOptimize(price);
    state.SetItemsProcessed(n_sims);
  }

}
BENCHMARK(BM_100MSimsSIMDConcurrentEuropean1Step)->UseRealTime();
static void BM_1BSimsSIMDConcurrentEuropean1Step(benchmark::State& state) {
  MarketData market(
      100.0, // Spot
      0.05,  // Risk free rate
      0.20   // Volatility
      );


  int n_steps = 1;
  int n_sims = 1000000000;
  double strike = 100.0;
  double expiry = 1.0; // Years decimal

  Params params(n_steps, n_sims, params::UnderlyingModel::GBM, params::NormalMethod::BoxMuller);

  std::unique_ptr<Option> option =
    std::make_unique<EuropeanOption>(strike, expiry, EuropeanOption::OptionType::Call);

  MCPricer pricer{params};

  for(auto _ : state) {
    double price = pricer.price(*option, market);
    benchmark::DoNotOptimize(price);
    state.SetItemsProcessed(n_sims);
  }

}
BENCHMARK(BM_1BSimsSIMDConcurrentEuropean1Step)->UseRealTime();

static void BM_100MSimsSIMDConcurrentAsian252Step(benchmark::State& state) {
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
    std::make_unique<AsianOption>(strike, expiry, AsianOption::OptionType::Call, AsianOption::StrikeType::Fixed, 0.25f);

  MCPricer pricer{params};

  for(auto _ : state) {
    double price = pricer.price(*option, market);
    benchmark::DoNotOptimize(price);
    state.SetItemsProcessed(n_sims);
  }
}
BENCHMARK(BM_100MSimsSIMDConcurrentAsian252Step)->UseRealTime();
