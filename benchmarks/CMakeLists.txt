add_executable(benchmarks
    benchmarks_rng.cpp
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
