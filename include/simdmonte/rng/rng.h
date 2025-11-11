#pragma once
#include "simdmonte/pricer/params.h"
#include <cstdint>
#include <immintrin.h>
namespace simdmonte {

// const functions and mutable members are required to keep pricer functions
// const
class Rng {
public:
  Rng();
  Rng(uint64_t seed);
  __m256 uniform() const;
  __m256 normal(params::NormalMethod method) const;

private:
  mutable __m256i state0_;
  mutable __m256i state1_;
  __m256i advance_state() const;

  __m256 box_muller_transform() const;
  mutable bool bm_cached = false;
  mutable __m256 bm_cached_values;

  __m256 inverse_cdf_approx() const;
  __m256 vec_u64_to_float() const;
};
} // namespace simdmonte
