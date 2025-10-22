#include "simdmonte/option/option_european.h"
#include "simdmonte/option/option.h"
#include "simdmonte/simdhelper/simdhelper_european.h"
#include <cmath>

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
std::unique_ptr<ISimdHelper> EO::get_simd_helper() const {
  return std::make_unique<EuropeanSimdHelper>(strike, type);
}
