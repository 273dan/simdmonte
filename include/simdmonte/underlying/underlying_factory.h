#pragma once
#include <memory>
#include <stdexcept>

#include "simdmonte/underlying/underlying_gbmlog.h"

namespace simdmonte {

class UnderlyingFactory {
 public:
  static std::unique_ptr<IUnderlying>
  create(const Option& option, const MarketData& market, const Params& params) {
    switch (params.underlying_model) {
      case params::UnderlyingModel::GBM:
        return std::make_unique<GBMLogUnderlying>(option, market, params);
    }

    throw std::invalid_argument("Unknown underlying model type");
  }
};
}  // namespace simdmonte
