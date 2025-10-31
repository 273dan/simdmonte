#pragma once
#include "simdmonte/accumulator/accumulator.h"
#include "simdmonte/misc/price_types.h"
#include "simdmonte/option/option_asian.h"
#include <immintrin.h>
namespace simdmonte {

class AsianAccumulator : public IAccumulator {
public:
  virtual void update(const LogSpaceVec&) override;
  virtual void update(const PriceSpaceVec&) override;
  virtual __m256 payoffs() override;
  virtual void reset() override;
  AsianAccumulator(float strike, AsianOption::OptionType o_type, AsianOption::StrikeType s_type, int avg_start);


private:
  __m256 avg_;
  int avg_start_;
  int steps_elapsed_;
  int steps_priced_;
  PriceSpaceVec sum_;
  PriceSpaceVec current_;
  __m256 strike_;

  AsianOption::OptionType o_type_;
  AsianOption::StrikeType s_type_;

  void accumulate(const PriceSpaceVec& prices);

};
} // namespace simdmonte

