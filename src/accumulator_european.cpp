#include "simdmonte/accumulator/accumulator_european.h"
namespace simdmonte {


EuropeanAccumulator::EuropeanAccumulator(float strike, EuropeanOption::OptionType type) :
  type_(type), strikes_(_mm256_set1_ps(strike)), zeroes_(_mm256_setzero_ps()) {};


void EuropeanAccumulator::update(const __m256& prices) {
  prices_ = prices;
}

__m256 EuropeanAccumulator::payoffs() {
  __m256 payoffs;
  if(type_ == EuropeanOption::OptionType::Call) {
    payoffs = _mm256_sub_ps(prices_, strikes_);
  }
  else {
    payoffs = _mm256_sub_ps(strikes_, prices_);
  }

  return _mm256_max_ps(payoffs, zeroes_);
  
}
}
  
