add_executable(benchmarks
    benchmarks_euro.cpp
    benchmarks_sisd_euro.cpp
    benchmarks_asian.cpp
    benchmarks_rng.cpp
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
