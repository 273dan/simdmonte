#pragma once
#include <cstdint>
namespace simdmonte {
namespace params {
  enum class UnderlyingModel {
    GBM
  };
  enum class NormalMethod {
    BoxMuller,
    InverseCDF
  };

} // namespace params

struct Params {
  int n_steps;
  long n_sims;
  int64_t seed = 0;
  params::UnderlyingModel underlying_model = params::UnderlyingModel::GBM;
  params::NormalMethod normal_method = params::NormalMethod::BoxMuller;
  // default constructor only here for use in gtest -- should find another way
  Params() {};
  Params(int n_steps_, int n_sims_, params::UnderlyingModel underlying_model_, params::NormalMethod normal_method_) :
    n_steps(n_steps_), n_sims(n_sims_), underlying_model(underlying_model_), normal_method(normal_method_) {};
};


}
