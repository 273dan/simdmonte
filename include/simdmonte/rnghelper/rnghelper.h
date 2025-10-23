#pragma once
#include <immintrin.h>
#include <cstdint>
namespace simdmonte {



// Const functions and mutable members are required to keep pricer functions const
class RngHelper {
public:
  RngHelper();
  RngHelper(uint64_t seed);
  __m256 unif_floats_8() const;
  __m256 normal_floats_8() const;

private:
  mutable __m256i state0_;
  mutable __m256i state1_;
  mutable bool cached = false;
  mutable __m256 cached_normals;
  __m256i advance_state() const;



};
}
