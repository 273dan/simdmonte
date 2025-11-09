#include <sstream>
#include <iomanip>
#include <benchmark/benchmark.h>
#include "simdmonte/misc/market_data.h"
#include "simdmonte/option/option_asian.h"
#include "simdmonte/pricer/params.h"
#include "simdmonte/pricer/pricer_sisd.h"
using namespace simdmonte;

static void BM_SISD_Asian_Performance(benchmark::State& state) {

  MarketData market{
      100.0, // Spot
      0.05,  // Risk free rate
      0.20   // Volatility
  };
  long bm_n_sims = state.range(0);
  Params bm_params{
    252, // n steps (252 for Asian option)
    bm_n_sims, 
    simdmonte::params::UnderlyingModel::GBM,
    simdmonte::params::NormalMethod::BoxMuller

  };

  AsianOption option{100.0, 1.0, simdmonte::AsianOption::OptionType::Call, simdmonte::AsianOption::StrikeType::Fixed, 0.5f};
  MCPricerSISD pricer{bm_params};

  for(auto _: state) {
    float price = pricer.price(option, market);
    benchmark::DoNotOptimize(price);
    state.SetItemsProcessed(bm_n_sims);
  }

  std::stringstream ss;
  ss << std::scientific 
     << std::setprecision(0) 
     << static_cast<double>(bm_n_sims); 

  state.SetLabel(ss.str() + " sims.");
}

BENCHMARK(BM_SISD_Asian_Performance)
  ->RangeMultiplier(10)
  ->Range(1e7, 1e8)
  ->UseRealTime();
