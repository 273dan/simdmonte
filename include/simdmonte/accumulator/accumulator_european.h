#pragma once
#include "simdmonte/accumulator/accumulator.h"
#include "simdmonte/option/option_european.h"
#include <immintrin.h>
namespace simdmonte {


class EuropeanAccumulator : public IAccumulator {
public:
  virtual void update(const LogSpaceVec&) override;
  virtual void update(const PriceSpaceVec&) override;
  virtual __m256 payoffs() override;
  virtual void reset() override;
  EuropeanAccumulator(float strike, EuropeanOption::OptionType type);
private:
  __m256 prices_;
  __m256 strikes_;
  __m256 zeroes_;
  EuropeanOption::OptionType type_;
  


};
}
