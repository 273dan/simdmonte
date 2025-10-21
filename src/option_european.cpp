#include "option_european.h"
#include "option.h"

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
