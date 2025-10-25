#include "simdmonte/pricer/pricer_simd.h"
#include "simdmonte/rng/rng.h"
#include "simdmonte/misc/utils.h"
#include <cmath>
#include <immintrin.h>
#include <iostream>
namespace simdmonte {




MCPricerSIMD::MCPricerSIMD(int sims, int steps)
  : n_sims(sims), n_steps(steps) {};


float MCPricerSIMD::price(const Option& option, const MarketData& market) const {
  // Hard code log prices for european option/GBM for now
  float dt = option.expiry / static_cast<float>(n_steps);
  float drift = (market.risk_free_rate - (0.5 * market.volatility * market.volatility)) * dt;
  float vol_dt = std::sqrt(dt) * market.volatility;
  float sum_payoffs = 0.0;

  float log_drift = std::log(drift);
  float log_spot = std::log(market.spot);

  __m256 vols = _mm256_set1_ps(vol_dt);
  __m256 drifts = _mm256_set1_ps(drift);



  Rng rng_helper = Rng();
  for(int i = 0; i < n_sims; i += 8) {
    std::unique_ptr<ISimdHelper> helper = option.get_simd_helper();
    __m256 current_log_prices = _mm256_set1_ps(log_spot);
    helper->update(current_log_prices);
    for(int j = 0; j < n_steps; j++) {
      __m256 Z = rng_helper.normal(Rng::NormalMethod::InverseCDF);
      __m256 shocks = _mm256_mul_ps(vols, Z);
      current_log_prices = _mm256_add_ps(current_log_prices, shocks);
      current_log_prices = _mm256_add_ps(current_log_prices, drifts);
      helper->update(current_log_prices);
    }

    // may need to make european simd helper natrually handle logs for exotic support

    float log_final_prices[8];
    float final_prices[8];

    _mm256_storeu_ps(log_final_prices, current_log_prices);

    for(int i = 0; i < 8; i++){
      final_prices[i] = std::exp(log_final_prices[i]);
    }

    __m256 final_prices_simd = _mm256_loadu_ps(final_prices);

    // this should be optimised away
    

    helper->update(final_prices_simd);
    __m256 payoffs_simd = helper->payoffs();

    float payoffs[8];


    _mm256_storeu_ps(payoffs, payoffs_simd);

    sum_payoffs += payoffs[0] + payoffs[1] + payoffs[2] + payoffs[3] + payoffs[4] + payoffs[5] + payoffs[6] + payoffs[7];
    if(i % 10000 == 0) {
      // std::cout << "Simulation: " << i << " / " << n_sims << "\n";
    }
  }

  float average_payoff = sum_payoffs / static_cast<float>(n_sims);
  float discounted = average_payoff * std::exp(-market.risk_free_rate * option.expiry);
  return discounted;


}
}
