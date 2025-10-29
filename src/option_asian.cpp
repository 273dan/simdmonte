#include "simdmonte/option/option_asian.h"
#include "simdmonte/accumulator/accumulator_asian.h"

namespace simdmonte {

AsianOption::AsianOption(float K, float T, OptionType o_type, StrikeType s_type) : Option(K, T), option_type(o_type), strike_type(s_type) {};

std::unique_ptr<IAccumulator> AsianOption::get_accumulator() const {
  return std::make_unique<AsianAccumulator>(strike, option_type, strike_type);

}






}
