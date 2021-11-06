cmake_minimum_required(VERSION 3.13)
project(hw2 C)
project(tests CXX)


set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)

add_compile_options(-Wall -Wextra -pedantic)

add_compile_options(-pthread) 
add_link_options(-pthread)


option(WITH_COVERAGE "Add coverage estimation")
if(WITH_COVERAGE)
    add_compile_options(--coverage)
    add_link_options(--coverage)
endif()

option(WITH_SANITIZERS "Add address sanitizer")
if(WITH_SANITIZERS)
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
endif()


set(INPUT input.c)
add_library(input_lib STATIC ${INPUT})

set(NAIVE_TRANSPOSE naive_transpose.c)
add_library(naive_transpose_lib STATIC ${NAIVE_TRANSPOSE})

set(PARALLEL_TRANSPOSE parallel_transpose.c)
add_library(parallel_transpose_lib SHARED ${PARALLEL_TRANSPOSE})

find_package(Threads REQUIRED)
add_executable(hw2 main.c)
target_link_libraries(hw2 input_lib naive_transpose_lib parallel_transpose_lib)


find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})


add_executable(test_input test_input.cpp)
target_link_libraries(test_input input_lib ${GTEST_LIBRARIES} pthread)


add_executable(test_naive_transpose test_naive_transpose.cpp)
target_link_libraries(test_naive_transpose naive_transpose_lib ${GTEST_LIBRARIES} pthread)


add_executable(test_parallel_transpose test_parallel_transpose.cpp)
target_link_libraries(test_parallel_transpose dl ${GTEST_LIBRARIES} pthread)