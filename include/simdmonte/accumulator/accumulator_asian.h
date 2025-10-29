#pragma once
#include "simdmonte/accumulator/accumulator.h"
#include "simdmonte/option/option_asian.h"
#include <immintrin.h>
namespace simdmonte {

class AsianAccumulator : public IAccumulator {
public:
  virtual void update(const __m256&) override;
  virtual __m256 payoffs() override;
  AsianAccumulator(float strike, AsianOption::OptionType o_type, AsianOption::StrikeType s_type);


private:
  __m256 avg_;
  __m256 sum_;
  int steps_total_;
  int step_thresh_;
  __m256i steps_priced_;
  float strike_;

  AsianOption::OptionType o_type_;
  AsianOption::StrikeType s_type_;

};
}

