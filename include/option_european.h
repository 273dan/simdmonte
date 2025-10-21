#pragma once
#include "option.h"
#include <vector>

class EuropeanOption : public Option {
public:
  enum class OptionType { Call, Put };
  OptionType type;
  EuropeanOption(double K, double T, OptionType type);
  virtual double payoff(const std::vector<double>& path) const override;
};
