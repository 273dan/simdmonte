add_library(simdmonte_lib
    option.cpp
    option_european.cpp
    option_asian.cpp
    pricer.cpp
    accumulator.cpp
    accumulator_european.cpp
    accumulator_asian.cpp
    rng.cpp
    utils.cpp
    underlying.cpp
    underlying_gbmlog.cpp
    avx_mathfun_wrapper.cpp
)

target_include_directories(simdmonte_lib
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include

)

target_compile_options(simdmonte_lib
    PRIVATE
    -mavx2
    -mfma
)
