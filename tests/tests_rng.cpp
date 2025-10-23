#include <gtest/gtest.h>
#include <immintrin.h>
#include <simdmonte/rnghelper/rnghelper.h>


using namespace simdmonte;

TEST(RngTests, UniformRange) {
  RngHelper rng{};
  float result[8];
  for(int i = 0; i < 100000; i++) {
    __m256 floats = rng.unif_floats_8();
    _mm256_storeu_ps(result, floats);
    for(auto j: result) {
      ASSERT_GE(j, 0.0f);
      ASSERT_LE(j, 1.0f);

    }

  }
}

TEST(RngTests, UniformSeeding) {
  RngHelper rng1(5555);
  RngHelper rng2(5555);

  float arr1[8], arr2[8];

  for(int i = 0; i < 100000; i++) {
    __m256 floats1 = rng1.unif_floats_8();
    __m256 floats2 = rng2.unif_floats_8();

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
  RngHelper rng{};
  const int n_calls = 100000;
  const int total_floats = 8 * n_calls;

  for(int i = 0; i < n_calls; i++) {
    __m256 floats = rng.unif_floats_8();
    _mm256_storeu_ps(arr, floats);
    for(auto j: arr) total += j;
  }

  double mean = total / static_cast<double>(total_floats);

  EXPECT_NEAR(mean, 0.5, 0.001);
}


TEST(RngTests, NormalSeeding) {
  RngHelper rng1(5555);
  RngHelper rng2(5555);

  float arr1[8], arr2[8];

  for(int i = 0; i < 100000; i++) {
    __m256 floats1 = rng1.normal_floats_8();
    __m256 floats2 = rng2.normal_floats_8();

    _mm256_storeu_ps(arr1, floats1);
    _mm256_storeu_ps(arr2, floats2);

    for(int j = 0; j < 8; j++) {
      EXPECT_EQ(arr1[j], arr2[j]);
    }
  }
}
TEST(RngTests, NormalMean) {
  double total = 0.0;
  float arr[8];
  RngHelper rng{};
  const int n_calls = 100000;
  const int total_floats = 8 * n_calls;

  for(int i = 0; i < n_calls; i++) {
    __m256 floats = rng.normal_floats_8();
    _mm256_storeu_ps(arr, floats);
    for(auto j: arr) total += j;
  }

  double mean = total / static_cast<double>(total_floats);

  EXPECT_NEAR(mean, 0.0, 0.001);
}
