#include <gtest/gtest.h>
#include <immintrin.h>
#include "simdmonte/rng/rng.h"


using namespace simdmonte;

TEST(RngTests, UniformRange) {
  Rng rng{};
  float result[8];
  for(int i = 0; i < 1000000; i++) {
    __m256 floats = rng.uniform();
    _mm256_storeu_ps(result, floats);
    for(auto j: result) {
      ASSERT_GE(j, 0.0f);
      ASSERT_LE(j, 1.0f);

    }

  }
}

TEST(RngTests, UniformSeeding) {
  Rng rng1(5555);
  Rng rng2(5555);

  float arr1[8], arr2[8];

  for(int i = 0; i < 1000000; i++) {
    __m256 floats1 = rng1.uniform();
    __m256 floats2 = rng2.uniform();

    _mm256_storeu_ps(arr1, floats1);
    _mm256_storeu_ps(arr2, floats2);

    for(int j = 0; j < 8; j++) {
      EXPECT_EQ(arr1[j], arr2[j]);
    }
  }

}

TEST(RngTests, UniformMean) {
  double total = 0.0;
  float arr[8];
  Rng rng{};
  const int n_calls = 10000000;
  const int total_floats = 8 * n_calls;

  for(int i = 0; i < n_calls; i++) {
    __m256 floats = rng.uniform();
    _mm256_storeu_ps(arr, floats);
    for(auto j: arr) total += j;
  }

  double mean = total / static_cast<double>(total_floats);

  EXPECT_NEAR(mean, 0.5, 0.001);
}
TEST(RngTests, SIMDUniformRange) {
  Rng rng{};
  float result[8];
  for(int i = 0; i < 1000000; i++) {
    __m256 floats = rng.uniform_simd();
    _mm256_storeu_ps(result, floats);
    for(auto j: result) {
      ASSERT_GE(j, 0.0f);
      ASSERT_LE(j, 1.0f);

    }

  }
}

TEST(RngTests, SIMDUniformSeeding) {
  Rng rng1(5555);
  Rng rng2(5555);

  float arr1[8], arr2[8];

  for(int i = 0; i < 1000000; i++) {
    __m256 floats1 = rng1.uniform_simd();
    __m256 floats2 = rng2.uniform_simd();

    _mm256_storeu_ps(arr1, floats1);
    _mm256_storeu_ps(arr2, floats2);

    for(int j = 0; j < 8; j++) {
      EXPECT_EQ(arr1[j], arr2[j]);
    }
  }

}

TEST(RngTests, SIMDUniformMean) {
  double total = 0.0;
  float arr[8];
  Rng rng{};
  const int n_calls = 10000000;
  const int total_floats = 8 * n_calls;

  for(int i = 0; i < n_calls; i++) {
    __m256 floats = rng.uniform_simd();
    _mm256_storeu_ps(arr, floats);
    for(auto j: arr) total += j;
  }

  double mean = total / static_cast<double>(total_floats);

  EXPECT_NEAR(mean, 0.5, 0.001);
}
TEST(RngTests, SIMDOneVecUniformRange) {
  Rng rng{};
  float result[8];
  for(int i = 0; i < 1000000; i++) {
    __m256 floats = rng.uniform_simd_one_vec();
    _mm256_storeu_ps(result, floats);
    for(auto j: result) {
      ASSERT_GE(j, 0.0f);
      ASSERT_LE(j, 1.0f);

    }

  }
}

TEST(RngTests, SIMDOneVecUniformSeeding) {
  Rng rng1(5555);
  Rng rng2(5555);

  float arr1[8], arr2[8];

  for(int i = 0; i < 1000000; i++) {
    __m256 floats1 = rng1.uniform_simd_one_vec();
    __m256 floats2 = rng2.uniform_simd_one_vec();

    _mm256_storeu_ps(arr1, floats1);
    _mm256_storeu_ps(arr2, floats2);

    for(int j = 0; j < 8; j++) {
      EXPECT_EQ(arr1[j], arr2[j]);
    }
  }

}

TEST(RngTests, SIMDOneVecUniformMean) {
  double total = 0.0;
  float arr[8];
  Rng rng{};
  const int n_calls = 10000000;
  const int total_floats = 8 * n_calls;

  for(int i = 0; i < n_calls; i++) {
    __m256 floats = rng.uniform_simd_one_vec();
    _mm256_storeu_ps(arr, floats);
    for(auto j: arr) total += j;
  }

  double mean = total / static_cast<double>(total_floats);

  EXPECT_NEAR(mean, 0.5, 0.001);
}


TEST(RngTests, BoxMullerNormalSeeding) {
  Rng rng1(5555);
  Rng rng2(5555);

  float arr1[8], arr2[8];

  for(int i = 0; i < 1000000; i++) {
    __m256 floats1 = rng1.normal(params::NormalMethod::BoxMuller);
    __m256 floats2 = rng2.normal(params::NormalMethod::BoxMuller);

    _mm256_storeu_ps(arr1, floats1);
    _mm256_storeu_ps(arr2, floats2);

    for(int j = 0; j < 8; j++) {
      EXPECT_EQ(arr1[j], arr2[j]);
    }
  }
}
TEST(RngTests, BoxMullerNormalMean) {
  double total = 0.0;
  float arr[8];
  Rng rng{};
  const int n_calls = 10000000;
  const int total_floats = 8 * n_calls;

  for(int i = 0; i < n_calls; i++) {
    __m256 floats = rng.normal(params::NormalMethod::BoxMuller);
    _mm256_storeu_ps(arr, floats);
    for(auto j: arr) total += j;
  }

  double mean = total / static_cast<double>(total_floats);

  EXPECT_NEAR(mean, 0.0, 0.001);
}
TEST(RngTests, InverseCDFNormalSeeding) {
  Rng rng1(5555);
  Rng rng2(5555);

  float arr1[8], arr2[8];

  for(int i = 0; i < 100000; i++) {
    __m256 floats1 = rng1.normal(params::NormalMethod::InverseCDF);
    __m256 floats2 = rng2.normal(params::NormalMethod::InverseCDF);

    _mm256_storeu_ps(arr1, floats1);
    _mm256_storeu_ps(arr2, floats2);

    for(int j = 0; j < 8; j++) {
      EXPECT_EQ(arr1[j], arr2[j]);
    }
  }
}
TEST(RngTests, InverseCDFNormalMean) {
  double total = 0.0;
  float arr[8];
  Rng rng{};
  const int n_calls = 10000000;
  const int total_floats = 8 * n_calls;

  for(int i = 0; i < n_calls; i++) {
    __m256 floats = rng.normal(params::NormalMethod::InverseCDF);
    _mm256_storeu_ps(arr, floats);
    for(auto j: arr) total += j;
  }

  double mean = total / static_cast<double>(total_floats);

  EXPECT_NEAR(mean, 0.0, 0.001);
}
