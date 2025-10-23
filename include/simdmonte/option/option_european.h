#pragma once
#include "simdmonte/option/option.h"
#include <vector>
namespace simdmonte {


class EuropeanOption : public Option {
public:
  enum class OptionType { Call, Put };
  OptionType type;
  EuropeanOption(float K, float T, OptionType type);
  virtual float payoff(const std::vector<float>& path) const override;
  virtual std::unique_ptr<ISimdHelper> get_simd_helper() const override;
};
}
