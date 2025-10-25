#include <benchmark/benchmark.h>
#include "simdmonte/rng/rng.h"
#include <random>

static void BM_STL_Uniform(benchmark::State& state) {
  std::mt19937 gen(std::random_device{}());
  std::uniform_real_distribution<float> dist{0.0f, 1.0f};

  for(auto _: state) {
      float val = dist(gen);
      benchmark::DoNotOptimize(val);
      val = dist(gen);
      benchmark::DoNotOptimize(val);
      val = dist(gen);
      benchmark::DoNotOptimize(val);
      val = dist(gen);
      benchmark::DoNotOptimize(val);
      val = dist(gen);
      benchmark::DoNotOptimize(val);
      val = dist(gen);
      benchmark::DoNotOptimize(val);
      val = dist(gen);
      benchmark::DoNotOptimize(val);
      val = dist(gen);
      benchmark::DoNotOptimize(val);
  }

}
BENCHMARK(BM_STL_Uniform);

static void BM_Uniform(benchmark::State& state) {
  simdmonte::Rng helper{};

  for (auto _: state) {
    __m256 result = helper.uniform();
    benchmark::DoNotOptimize(result);
  }


}
BENCHMARK(BM_Uniform);

static void BM_STL_Normal(benchmark::State& state) {
  std::mt19937 gen(std::random_device{}());
  std::normal_distribution<float> dist{0.0f, 1.0f};
  // run 8 times per iteration as SIMD functions generate 8 numbers per iteration
  for(auto _: state) {
      float val = dist(gen);
      benchmark::DoNotOptimize(val);
      val = dist(gen);
      benchmark::DoNotOptimize(val);
      val = dist(gen);
      benchmark::DoNotOptimize(val);
      val = dist(gen);
      benchmark::DoNotOptimize(val);
      val = dist(gen);
      benchmark::DoNotOptimize(val);
      val = dist(gen);
      benchmark::DoNotOptimize(val);
      val = dist(gen);
      benchmark::DoNotOptimize(val);
      val = dist(gen);
      benchmark::DoNotOptimize(val);


  }
}
BENCHMARK(BM_STL_Normal);

static void BM_BoxMullerNormal(benchmark::State& state) {
  simdmonte::Rng rng{};

  for (auto _: state) {
    __m256 result = rng.normal(simdmonte::Rng::NormalMethod::BoxMuller);
    benchmark::DoNotOptimize(result);
  }


}
BENCHMARK(BM_BoxMullerNormal);

static void BM_InverseCDFNormal(benchmark::State& state) {
  simdmonte::Rng rng{};

  for (auto _: state) {
    __m256 result = rng.normal(simdmonte::Rng::NormalMethod::InverseCDF);
    benchmark::DoNotOptimize(result);
  }


}
BENCHMARK(BM_InverseCDFNormal);



BENCHMARK_MAIN();
