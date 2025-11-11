#pragma once
#include "simdmonte/option/option.h"
#include <memory>
namespace simdmonte {

class EuropeanOption : public Option {
public:
  enum class OptionType { Call, Put };
  OptionType type;
  EuropeanOption(float K, float T, OptionType type);
  virtual std::unique_ptr<IAccumulator>
  get_accumulator(const Params &params) const override;

  // FOR GTEST ONLY
  EuropeanOption() {}
};
} // namespace simdmonte
