#pragma once
#include <memory>

#include "simdmonte/accumulator/accumulator.h"
#include "simdmonte/pricer/params.h"
namespace simdmonte {

class Option {
 public:
  virtual ~Option() = 0;
  virtual std::unique_ptr<IAccumulator>
  get_accumulator(const Params& params) const = 0;

  float strike;
  float expiry;  // 1.0 = 1 year
                 //
                 //

 protected:
  Option(float K, float T);

  // FOR GTEST ONLY
  Option() {};
};

}  // namespace simdmonte
