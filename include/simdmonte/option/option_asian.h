#pragma once
#include "simdmonte/option/option.h"
namespace simdmonte {
class AsianOption : public Option {
public:
  enum class OptionType {Call, Put};
  enum class StrikeType {Fixed, Floating};
  OptionType option_type;
  StrikeType strike_type;
  AsianOption(float K, float T, OptionType o_type, StrikeType s_type);
  virtual std::unique_ptr<IAccumulator> get_accumulator() const override;
};
}
