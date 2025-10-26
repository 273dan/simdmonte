#include "simdmonte/pricer/pricer_simd.h"
#include "simdmonte/pricer/params.h"
#include "simdmonte/rng/rng.h"
#include "simdmonte/misc/utils.h"
#include "simdmonte/underlying/underlying.h"
#include "simdmonte/underlying/underlying_factory.h"
#include <cmath>
#include <immintrin.h>
#include <iostream>
namespace simdmonte {




MCPricerSIMD::MCPricerSIMD(Params params) :
  params_(params), n_steps_(params.n_steps), n_sims_(params.n_sims) {};


float MCPricerSIMD::price(const Option& option, const MarketData& market) const {

  std::unique_ptr<IUnderlying> underlying = UnderlyingFactory::create(option, market, params_);
  std::unique_ptr<ISimdHelper> accumulator = option.get_simd_helper();

  float sum_payoffs = 0.0f;

  for(int sim = 0; sim < n_sims_ + 8; sim += 8) {
    underlying->set_current(std::log(market.spot));

    for(int step = 0; step < n_steps_; step++) {
      accumulator->update(underlying->step());
    }
    accumulator->update(underlying->get_special());
    __m256 payoffs = accumulator->payoffs();
    float payoffs_arr[8];
    _mm256_storeu_ps(payoffs_arr, payoffs);
    for(auto p: payoffs_arr) {
      sum_payoffs += p;
    }
  }

  float average_payoff = sum_payoffs / static_cast<float>(n_sims_);
  float discounted = average_payoff * std::exp(-market.risk_free_rate * option.expiry);
  return discounted;


}
}
