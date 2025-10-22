#pragma once
#include <immintrin.h>
#include <random>


// Const functions and mutable members are required to keep pricer functions const
class RngHelper {
public:
  RngHelper();
  static float u64_to_float(uint64_t n);
  __m256 unif_floats_8() const;
  __m256 normal_floats_8() const;

private:
  mutable __m256i state0_;
  mutable __m256i state1_;
  __m256i advance_state() const;



};
