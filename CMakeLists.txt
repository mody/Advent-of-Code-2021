cmake_minimum_required(VERSION 3.11)
project(AOC2021)

set(CMAKE_CXX_FLAGS "-Wall -std=c++17 -fno-omit-frame-pointer -ggdb3 -O3")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# find_package(PkgConfig)
# pkg_check_modules(PC_RapidJSON QUIET RapidJSON)

#  find_package(TBB REQUIRED tbb)

# set(BOOST_ROOT "/home/ov699/opt/")
#  find_package(Boost 1.67.0 COMPONENTS unit_test_framework REQUIRED)
#  include_directories(src ${Boost_INCLUDE_DIR} ${TBB_DIR})

add_executable(day1 src/day1.cc)
target_compile_features(day1 PRIVATE cxx_std_17)

add_executable(day2 src/day2.cc)
target_compile_features(day2 PRIVATE cxx_std_17)

add_executable(day3 src/day3.cc)
target_compile_features(day3 PRIVATE cxx_std_17)

add_executable(day4 src/day4.cc)
target_compile_features(day4 PRIVATE cxx_std_17)

add_executable(day5 src/day5.cc)
target_compile_features(day5 PRIVATE cxx_std_17)

add_executable(day6 src/day6.cc)
target_compile_features(day6 PRIVATE cxx_std_17)
