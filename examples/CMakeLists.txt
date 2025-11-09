add_executable(euro_option euro_option.cpp)
add_executable(asian_option_sisd sisd_asian_option.cpp)

find_package(Threads REQUIRED)

target_link_libraries(euro_option
    PRIVATE
    simdmonte_lib
    Threads::Threads
)
target_link_libraries(asian_option_sisd
    PRIVATE
    simdmonte_lib
    Threads::Threads
)
