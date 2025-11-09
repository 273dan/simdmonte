#include "simdmonte/pricer/pricer.h"
#include "simdmonte/pricer/params.h"
#include "simdmonte/misc/utils.h"
#include "simdmonte/underlying/underlying.h"
#include "simdmonte/underlying/underlying_factory.h"
#include <numeric>
#include <thread>
#include <cmath>
#include <immintrin.h>
#include <vector>
namespace simdmonte {




MCPricer::MCPricer(Params params) :
  params_(params), n_steps_(params.n_steps), n_sims_(params.n_sims) {};


float MCPricer::price(const Option& option, const MarketData& market) const {
  // Thread management
  unsigned int n_threads = std::thread::hardware_concurrency();
  n_threads = n_threads == 0 ? 1 : n_threads;

  int total_calls = (n_sims_ + 7) / 8;

  int per_thread = total_calls / n_threads; // Ensure that we do the exact number of simulations specified
  int remainder = total_calls % n_threads;


  std::vector<std::thread> threads{};
  threads.reserve(n_threads);

  std::vector<double> payoff_sums(n_threads, 0.0f);


  for(int i = 0; i < n_threads; i++) {
    int thread_calls = per_thread + (i < remainder ? 1 : 0);
    if(thread_calls == 0) continue;
    threads.emplace_back([=, &option, &market, &payoff_sums]{
      std::unique_ptr<IUnderlying> underlying = UnderlyingFactory::create(option, market, params_);
      std::unique_ptr<IAccumulator> accumulator = option.get_accumulator(params_);

      double sum_payoffs = 0.0;

      for(int sim = 0;  sim < thread_calls; sim ++) {
        underlying->set_current(std::log(market.spot)); // Set the starting price for the underlying

        for(int step = 0; step < n_steps_; step++) {
          accumulator->update(underlying->step()); // Step forward and inform accumulator of the new price
        }
        __m256 payoffs = accumulator->payoffs();
        float payoffs_arr[8];
        _mm256_storeu_ps(payoffs_arr, payoffs);
        for(auto p: payoffs_arr) {
          sum_payoffs += p;
        }
        accumulator->reset(); // Make sure that path-dependent variables are reset before the next simulation
      }

      payoff_sums[i] = sum_payoffs;
    });
  }

  for(auto& thread : threads) {
    if (thread.joinable()) thread.join();
  }

  float total_payoff_sum = std::accumulate(payoff_sums.begin(), payoff_sums.end(), 0.0f);
  float average_payoff = total_payoff_sum / static_cast<float>(total_calls * 8);
  float discounted = average_payoff * std::exp(-market.risk_free_rate * option.expiry);
  return discounted;


}
}
