#pragma once
#include "simdhelper.h"
#include "option_european.h"

class EuropeanSimdHelper : public ISimdHelper {
public:
  virtual void update(const __m256d&) override;
  virtual __m256d payoffs() override;
  EuropeanSimdHelper(double strike, EuropeanOption::OptionType type);
private:
  __m256d prices_;
  __m256d strikes_;
  __m256d zeroes_;
  EuropeanOption::OptionType type_;
  


};
