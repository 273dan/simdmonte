#include "option_european.h"
#include "option.h"
#include "simdhelper_european.h"
#include <cmath>

using EO = EuropeanOption;
EO::EuropeanOption(double K, double T, OptionType type)
    : Option(K, T), type(type){};

double EO::payoff(const std::vector<double>& path) const  {
  double final_price = path.back();
  if(type == OptionType::Call) {
    return std::max(final_price - strike, 0.0);

  }
  else {
    return std::max(strike - final_price, 0.0);
  }


}
std::unique_ptr<ISimdHelper> EO::get_simd_helper() const {
  return std::make_unique<EuropeanSimdHelper>(strike, type);
}
