#pragma once
#include "simdmonte/simdhelper/simdhelper.h"
#include "simdmonte/option/option_european.h"

class EuropeanSimdHelper : public ISimdHelper {
public:
  virtual void update(const __m256&) override;
  virtual __m256 payoffs() override;
  EuropeanSimdHelper(double strike, EuropeanOption::OptionType type);
private:
  __m256 prices_;
  __m256 strikes_;
  __m256 zeroes_;
  EuropeanOption::OptionType type_;
  


};
