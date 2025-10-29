#include "simdmonte/accumulator/accumulator_asian.h"
#include "simdmonte/option/option_asian.h"
#include "avx_mathfun/avx_mathfun.h"
#include <immintrin.h>
#include <iostream>

namespace simdmonte {

AsianAccumulator::AsianAccumulator(float strike, AsianOption::OptionType o_type, AsianOption::StrikeType s_type, int avg_start) :
  strike_(_mm256_set1_ps(strike)), o_type_(o_type), s_type_(s_type), avg_start_(avg_start), steps_priced_(0), steps_elapsed_(0), sum_(_mm256_setzero_ps()) {



  };

void AsianAccumulator::update(const __m256& prices_log_space) {
  steps_elapsed_++;
  if(steps_elapsed_ >= avg_start_) {
    steps_priced_++;
    __m256 prices_price_space = exp256_ps(prices_log_space);
    float pric[8];
    _mm256_storeu_ps(pric, prices_price_space);
    for(auto f: pric) {
      std::cout << f << std::endl;
    }
    current_ = prices_price_space;
    sum_ = _mm256_add_ps(sum_, prices_price_space);

  }
}

__m256 AsianAccumulator::payoffs() {
  __m256 steps_priced_vecf = _mm256_set1_ps(static_cast<float>(steps_priced_));
  __m256 avg = _mm256_div_ps(sum_, steps_priced_vecf);
  __m256 lhs, rhs;
  __m256 payoffs;
  __m256 zeroes = _mm256_setzero_ps();

  if (s_type_ == AsianOption::StrikeType::Fixed) {
    lhs = avg;
    rhs = strike_;
  }
  else {
    lhs = strike_;
    rhs = avg;
  }

  if (o_type_ == AsianOption::OptionType::Call) {
    payoffs = _mm256_sub_ps(lhs, rhs);
  }
  else {
    payoffs = _mm256_sub_ps(rhs, lhs);
  }

  return _mm256_max_ps(zeroes, payoffs);
}

void AsianAccumulator::reset() {
  sum_ = _mm256_setzero_ps();
  steps_elapsed_ = 0;
  steps_priced_ = 0;

}








}
