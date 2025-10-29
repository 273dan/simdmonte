#include "simdmonte/accumulator/accumulator_asian.h"
#include "simdmonte/option/option_asian.h"
#include <immintrin.h>

namespace simdmonte {

AsianAccumulator::AsianAccumulator(float strike, AsianOption::OptionType o_type, AsianOption::StrikeType s_type) : strike_(strike), o_type_(o_type), s_type_(s_type) {};

void AsianAccumulator::update(const __m256& prices) {
  step_ = _mm256_add_epi32(step_, _mm256_set1_epi32(1));

  

}








}
