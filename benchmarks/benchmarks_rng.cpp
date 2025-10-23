#include <benchmark/benchmark.h>
#include "simdmonte/rnghelper/rnghelper.h"
#include <random>

static void BM_Uniform(benchmark::State& state) {
  simdmonte::RngHelper helper{};

  for (auto _: state) {
    __m256 result = helper.unif_floats_8();
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
  simdmonte::RngHelper helper{};

  for (auto _: state) {
    __m256 result = helper.normal_floats_8();
    benchmark::DoNotOptimize(result);
  }


}
BENCHMARK(BM_BoxMullerNormal);




BENCHMARK_MAIN();
