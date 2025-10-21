#include "simdhelper_european.h"

EuropeanSimdHelper::EuropeanSimdHelper(double strike, EuropeanOption::OptionType type) :
  type_(type), strikes_(_mm256_set1_pd(strike)), zeroes_(_mm256_setzero_pd()) {};


void EuropeanSimdHelper::update(const __m256d& prices) {
  prices_ = prices;
}

__m256d EuropeanSimdHelper::payoffs() {
  __m256d payoffs;
  if(type_ == EuropeanOption::OptionType::Call) {
    payoffs = _mm256_sub_pd(prices_, strikes_);
  }
  else {
    payoffs = _mm256_sub_pd(strikes_, prices_);
  }

  return _mm256_max_pd(payoffs, zeroes_);
  
}
  
