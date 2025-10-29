#include "simdmonte/accumulator/accumulator_asian.h"
#include "simdmonte/option/option_asian.h"
#include <immintrin.h>

namespace simdmonte {

AsianAccumulator::AsianAccumulator(float strike, AsianOption::OptionType o_type, AsianOption::StrikeType s_type) : strike_(strike), o_type_(o_type), s_type_(s_type), steps_priced_(_mm256_set1_epi32(0)) {};

void AsianAccumulator::update(const __m256& prices) {
  steps_total_++;
  if(steps_total_ >= step_thresh_) {

  }



  

}








}
