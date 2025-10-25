#pragma once
#include <immintrin.h>
#include <cstdint>
namespace simdmonte {



// const functions and mutable members are required to keep pricer functions const
class Rng {
public:
  enum class NormalMethod {
    BoxMuller,
    InverseCDF
  };
  Rng();
  Rng(uint64_t seed);
  __m256 uniform() const;
  __m256 normal(NormalMethod method) const;

private:
  // u64 states for xorshift128+
  mutable __m256i state0_;
  mutable __m256i state1_;
  __m256i advance_state() const;

  // box muller will produce 16 values, for each call cache 8 and return the other 8
  __m256 box_muller_transform() const;
  mutable bool bm_cached = false;
  mutable __m256 bm_cached_values;

  __m256 inverse_cdf_approx() const;



};
}
