add_executable(tests 
    tests_rng.cpp
    tests_european_pricing.cpp
    tests_asian_pricing.cpp
    tests_SISD_euro_pricing.cpp
)

target_compile_options(tests
    PRIVATE
    -mavx2
)

target_link_libraries(tests
    PRIVATE
    simdmonte_lib
    GTest::gtest_main
)

include(GoogleTest)


gtest_discover_tests(tests)
