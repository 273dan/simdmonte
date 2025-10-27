#pragma once
#include <vector>
#include <memory>
#include "simdmonte/accumulator/accumulator.h"
namespace simdmonte {

class Option {
public:
  virtual ~Option() = 0;
  virtual float payoff(const std::vector<float>& paths) const = 0;
  virtual std::unique_ptr<IAccumulator> get_accumulator() const = 0;

  float strike;
  float expiry; // 1.0 = 1 year
protected:
  Option(float K, float T);
};

}
