#include "simdmonte/accumulator/accumulator_asian.h"
#include "simdmonte/misc/price_types.h"
#include "simdmonte/option/option_asian.h"
#include "avx_mathfun/avx_mathfun.h"
#include <immintrin.h>

namespace simdmonte {

AsianAccumulator::AsianAccumulator(float strike, AsianOption::OptionType o_type, AsianOption::StrikeType strike_type, int avg_start) :
  strike_(_mm256_set1_ps(strike)), option_type_(o_type), strike_type_(strike_type), avg_start_(avg_start), steps_priced_(0), steps_elapsed_(0), sum_{_mm256_setzero_ps()} {



  };

void AsianAccumulator::update(const LogSpaceVec& log_prices) {
  steps_elapsed_++;
  if(steps_elapsed_ > avg_start_) {
    accumulate(to_price_space(log_prices));

  }
}

void AsianAccumulator::update(const PriceSpaceVec& prices) {
  steps_elapsed_++;
  if(steps_elapsed_ > avg_start_) {
    accumulate(prices);

  }
}

void AsianAccumulator::accumulate(const PriceSpaceVec& prices) {
  steps_priced_++;

  current_ = prices;
  sum_.value = _mm256_add_ps(sum_.value, prices.value);

}

__m256 AsianAccumulator::payoffs() {
  __m256 steps_priced_vecf = _mm256_set1_ps(static_cast<float>(steps_priced_));
  PriceSpaceVec avg {_mm256_div_ps(sum_.value, steps_priced_vecf)};
  __m256 lhs, rhs;
  __m256 payoffs;
  __m256 zeroes = _mm256_setzero_ps();

  if (strike_type_ == AsianOption::StrikeType::Fixed) {
    lhs = avg.value;
    rhs = strike_;
  }
  else {
    lhs = current_.value;
    rhs = avg.value;
  }

  if (option_type_ == AsianOption::OptionType::Call) {
    payoffs = _mm256_sub_ps(lhs, rhs);
  }
  else {
    payoffs = _mm256_sub_ps(rhs, lhs);
  }

  return _mm256_max_ps(zeroes, payoffs);
}

void AsianAccumulator::reset() {
  sum_.value = _mm256_setzero_ps();
  steps_elapsed_ = 0;
  steps_priced_ = 0;

}








}
