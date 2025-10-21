add_executable(main main.cpp)

find_package(Threads REQUIRED)

target_link_libraries(main
    PRIVATE
    simdmonte_lib
    Threads::Threads
)
