add_executable(benchmarks
    benchmarks_rng.cpp
    benchmarks_pricing.cpp
)

target_include_directories(benchmarks
    PRIVATE
    ${googlebenchmark_SOURCE_DIR}/include
)
target_link_libraries(benchmarks
    PRIVATE
    simdmonte_lib
    benchmark::benchmark
)

target_compile_options(benchmarks
    PRIVATE
    -mavx2
)
