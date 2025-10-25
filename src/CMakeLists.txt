add_library(simdmonte_lib
    option.cpp
    option_european.cpp
    pricer_sisd.cpp
    pricer_simd.cpp
    pricer.cpp
    simdhelper.cpp
    simdhelper_european.cpp
    rng.cpp
    utils.cpp
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
