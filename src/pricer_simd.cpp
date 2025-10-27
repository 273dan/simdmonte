#include "simdmonte/pricer/pricer_simd.h"
#include "simdmonte/pricer/params.h"
#include "simdmonte/misc/utils.h"
#include "simdmonte/underlying/underlying.h"
#include "simdmonte/underlying/underlying_factory.h"
#include <thread>
#include <cmath>
#include <immintrin.h>
#include <iostream>
#include <chrono>
namespace simdmonte {




MCPricerSIMD::MCPricerSIMD(Params params) :
  params_(params), n_steps_(params.n_steps), n_sims_(params.n_sims) {};


float MCPricerSIMD::price(const Option& option, const MarketData& market) const {

  std::unique_ptr<IUnderlying> underlying = UnderlyingFactory::create(option, market, params_);
  std::unique_ptr<ISimdHelper> accumulator = option.get_simd_helper();

  float sum_payoffs = 0.0f;
  int sims_completed = 0;

  for(int sim = 0; sim < n_sims_ + 8; sim += 8) {
    sims_completed++;
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

  float average_payoff = sum_payoffs / static_cast<float>(sims_completed);
  float discounted = average_payoff * std::exp(-market.risk_free_rate * option.expiry);
  return discounted;
}
float MCPricerSIMD::price_concurrent(const Option& option, const MarketData& market) const {
  unsigned int n_threads = std::thread::hardware_concurrency();
  n_threads = n_threads == 0 ? 1 : n_threads;

  int total_calls = (n_sims_ + 7) / 8;

  int per_thread = total_calls / n_threads;
  int remainder = total_calls % n_threads;


  std::vector<std::thread> threads{};
  threads.reserve(n_threads);

  std::vector<float> payoff_sums(n_threads, 0.0f);

  for(int i = 0; i < n_threads; i++) {
    int thread_calls = per_thread + (i < remainder ? 0 : 1);
    if(thread_calls == 0) continue;
    threads.emplace_back([=, &option, &market, &payoff_sums]{
      auto start = std::chrono::steady_clock::now();



      std::unique_ptr<IUnderlying> underlying = UnderlyingFactory::create(option, market, params_);
      std::unique_ptr<ISimdHelper> accumulator = option.get_simd_helper();

      float sum_payoffs = 0.0f;

      for(int sim = 0;  sim < thread_calls; sim ++) {
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

      payoff_sums[i] = sum_payoffs;
      auto end = std::chrono::steady_clock::now();
      auto duration = end - start;
      auto ms_duration_cast = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
    });
  }

  for(auto& thread : threads) {
    if (thread.joinable()) thread.join();
  }

  float total_payoff_sum = std::accumulate(payoff_sums.begin(), payoff_sums.end(), 0);
  float average_payoff = total_payoff_sum / static_cast<float>(n_sims_);
  float discounted = average_payoff * std::exp(-market.risk_free_rate * option.expiry);
  return discounted;


}
}
