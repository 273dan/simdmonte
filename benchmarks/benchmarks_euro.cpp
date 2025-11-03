#include <benchmark/benchmark.h>
#include <memory>
#include "simdmonte/misc/market_data.h"
#include "simdmonte/option/option_european.h"
#include "simdmonte/pricer/params.h"
#include "simdmonte/pricer/pricer.h"
#include "simdmonte/pricer/pricer_sisd.h"
namespace {
  const simdmonte::MarketData g_market_data{
      100.0, // Spot
      0.05,  // Risk free rate
      0.20   // Volatility
  };
  const std::shared_ptr<simdmonte::Option> option =
    std::make_unique<simdmonte::EuropeanOption>(100.0f, 1.0f, simdmonte::EuropeanOption::OptionType::Call);
  simdmonte::Params params{
    1, // n steps (1 for European option)
    1, // n sims (to be replaced with parameterised benchmark)
    simdmonte::params::UnderlyingModel::GBM,
    simdmonte::params::NormalMethod::BoxMuller
  };
}
using namespace simdmonte;

static void BM_Euro_Performance(benchmark::State& state) {


}
