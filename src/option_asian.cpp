#include "simdmonte/option/option_asian.h"

#include "simdmonte/accumulator/accumulator_asian.h"

namespace simdmonte {

AsianOption::AsianOption(float K, float T, OptionType o_type, StrikeType s_type,
                         float avg_p)
    : Option(K, T), option_type(o_type), strike_type(s_type), avg_period(avg_p) {};

std::unique_ptr<IAccumulator>
AsianOption::get_accumulator(const Params& params) const {
  int averaging_period_start_step =
      static_cast<int>(((expiry - avg_period) / expiry) * params.n_steps);
  return std::make_unique<AsianAccumulator>(strike, option_type, strike_type,
                                            averaging_period_start_step);
}

}  // namespace simdmonte
