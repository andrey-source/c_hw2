cmake_minimum_required(VERSION 3.13)
project(hw2 C)
project(tests CXX)


set(CMAKE_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)


add_compile_options(-Wall -Wextra -pedantic)

add_compile_options(-pthread) 
add_link_options(-pthread)


option(ADDRESS_SANITIZER "Add address sanitizer")
option(UNDEFINED_SANITIZER "Add undefined sanitizer")
option(THREAD_SANITIZER "Add thread sanitizer")
option(LEAK_SANITIZER "Add leak sanitizer")

option(WITH_COVERAGE "Add coverage estimation")
option(parallel "Parallel calculate")

if(WITH_COVERAGE)
    add_compile_options(--coverage)
    add_link_options(--coverage)
endif()


if(ADDRESS_SANITIZER)
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
endif()
if(UNDEFINED_SANITIZER)
    add_compile_options(-fsanitize=undefined)
    add_link_options(-fsanitize=undefined)
endif()
if(THREAD_SANITIZER)
    add_compile_options(-fsanitize=thread)
    add_link_options(-fsanitize=thread)
endif()
if(LEAK_SANITIZER)
    add_compile_options(-fsanitize=leak)
    add_link_options(-fsanitize=leak)
endif()

add_library(input_lib STATIC src/input.c)
target_include_directories(input_lib PUBLIC include)

add_library(naive_transpose_lib STATIC src/naive_transpose.c)
target_include_directories(naive_transpose_lib PUBLIC include)

add_library(parallel_transpose_lib SHARED src/parallel_transpose.c)
target_include_directories(parallel_transpose_lib PUBLIC include)

find_package(Threads REQUIRED)
add_executable(hw2 src/main.c)



if(parallel)
    target_link_libraries(hw2 input_lib parallel_transpose_lib)
else()
    target_link_libraries(hw2 input_lib naive_transpose_lib)
endif()


find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})


add_executable(test_input tests/test_input.cpp)
target_link_libraries(test_input input_lib ${GTEST_LIBRARIES} pthread)


add_executable(test_naive_transpose tests/test_transpose.cpp)
target_link_libraries(test_naive_transpose naive_transpose_lib ${GTEST_LIBRARIES} pthread)


add_executable(test_parallel_transpose tests/test_transpose.cpp)
target_link_libraries(test_parallel_transpose parallel_transpose_lib ${GTEST_LIBRARIES} pthread)

enable_testing()
add_test(test_input test_input)
add_test(test_naive_transpose test_naive_transpose)
add_test(test_parallel_transpose test_parallel_transpose)