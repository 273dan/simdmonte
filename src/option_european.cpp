#include "simdmonte/option/option_european.h"
#include "simdmonte/option/option.h"
#include "simdmonte/accumulator/accumulator_european.h"
#include <cmath>
namespace simdmonte {


EuropeanOption::EuropeanOption(float K, float T, OptionType type)
    : Option(K, T), type(type){};

std::unique_ptr<IAccumulator> EuropeanOption::get_accumulator() const {
  return std::make_unique<EuropeanAccumulator>(strike, type);
}
}
