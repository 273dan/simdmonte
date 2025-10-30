#include "simdmonte/accumulator/accumulator_european.h"
namespace simdmonte {


EuropeanAccumulator::EuropeanAccumulator(float strike, EuropeanOption::OptionType type) :
  type_(type), strikes_{_mm256_set1_ps(strike)}, zeroes_(_mm256_setzero_ps()) {};


void EuropeanAccumulator::update(const LogSpaceVec& prices) {
  prices_ = prices;
}
void EuropeanAccumulator::update(const PriceSpaceVec& prices) {
  prices_ = to_log_space(prices);
}

__m256 EuropeanAccumulator::payoffs() {
  PriceSpaceVec price_space = to_price_space(prices_);
  __m256 payoffs;
  if(type_ == EuropeanOption::OptionType::Call) {
    payoffs = _mm256_sub_ps(price_space.value, strikes_.value);
  }
  else {
    payoffs = _mm256_sub_ps(strikes_.value, price_space.value);
  }

  return _mm256_max_ps(payoffs, zeroes_);
  
}

void EuropeanAccumulator::reset() {
  // we don't need to reset the european accumulator as it is not path dependent

}
}
  
