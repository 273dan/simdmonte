#include "pricer_simd.h"
#include <cmath>
#include <immintrin.h>
#include <iostream>



MCPricerSIMD::MCPricerSIMD(int sims, int steps)
  : n_sims(sims), n_steps(steps), dist_(0.0, 1.0), gen_(std::random_device{}())  {};


double MCPricerSIMD::price(const Option& option, const MarketData& market) const {
  // Hard code log prices for european option/GBM for now
  double dt = option.expiry / static_cast<double>(n_steps);
  double drift = (market.risk_free_rate - (0.5 * market.volatility * market.volatility)) * dt;
  double vol_dt = std::sqrt(dt) * market.volatility;
  double sum_payoffs = 0.0;

  double log_drift = std::log(drift);
  double log_spot = std::log(market.spot);

  __m256d vols = _mm256_set1_pd(vol_dt);
  __m256d drifts = _mm256_set1_pd(drift);



  for(int i = 0; i < n_sims; i += 4) {
    std::unique_ptr<ISimdHelper> helper = option.get_simd_helper();
    __m256d current_log_prices = _mm256_set1_pd(log_spot);
    helper->update(current_log_prices);
    for(int j = 0; j < n_steps; j++) {
      __m256d Z = packed_double_normals();
      __m256d shocks = _mm256_mul_pd(vols, Z);
      current_log_prices = _mm256_add_pd(current_log_prices, shocks);
      current_log_prices = _mm256_add_pd(current_log_prices, drifts);
      helper->update(current_log_prices);
    }

    // may need to make european simd helper natrually handle logs for exotic support

    double log_final_prices[4];
    double final_prices[4];

    _mm256_storeu_pd(log_final_prices, current_log_prices);

    for(int i = 0; i < 4; i++){
      final_prices[i] = std::exp(log_final_prices[i]);
    }

    __m256d final_prices_simd = _mm256_loadu_pd(final_prices);

    // this should be optimised away
    

    helper->update(final_prices_simd);
    __m256d payoffs_simd = helper->payoffs();

    double payoffs[4];


    _mm256_storeu_pd(payoffs, payoffs_simd);

    sum_payoffs += payoffs[0] + payoffs[1] + payoffs[2] + payoffs[3];
    if(i % 10000 == 0) {
      std::cout << "Simulation: " << i << " / " << n_sims << "\n";
    }
  }

  double average_payoff = sum_payoffs / static_cast<double>(n_sims);
  double discounted = average_payoff * std::exp(-market.risk_free_rate * option.expiry);
  return discounted;


}


__m256d MCPricerSIMD::packed_double_normals() const {
  double z1 = dist_(gen_);
  double z2 = dist_(gen_);
  double z3 = dist_(gen_);
  double z4 = dist_(gen_);


  return _mm256_set_pd(z1, z2, z3, z4);
}



