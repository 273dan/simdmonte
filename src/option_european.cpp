#include "simdmonte/option/option_european.h"
#include "simdmonte/option/option.h"
#include "simdmonte/accumulator/accumulator_european.h"
#include <cmath>
namespace simdmonte {


using EO = EuropeanOption;
EO::EuropeanOption(float K, float T, OptionType type)
    : Option(K, T), type(type){};

float EO::payoff(const std::vector<float>& path) const  {
  float final_price = path.back();
  if(type == OptionType::Call) {
    return std::max(final_price - strike, 0.0f);

  }
  else {
    return std::max(strike - final_price, 0.0f);
  }


}
std::unique_ptr<IAccumulator> EO::get_accumulator() const {
  return std::make_unique<EuropeanAccumulator>(strike, type);
}
}
