add_executable(euro_option euro_option.cpp)

find_package(Threads REQUIRED)

target_link_libraries(euro_option
    PRIVATE
    simdmonte_lib
    Threads::Threads
)
