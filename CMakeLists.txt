cmake_minimum_required(VERSION 3.11)
project(AOC2021)

set(CMAKE_CXX_FLAGS "-Wall -std=c++17 -fno-omit-frame-pointer -ggdb3 -O3")
# set(CMAKE_CXX_FLAGS "-Wall -std=c++17 -fno-omit-frame-pointer -ggdb3 -O3 -fsanitize=address")
# set(CMAKE_CXX_FLAGS "-Wall -std=c++17 -fno-omit-frame-pointer -ggdb3 -O3 -fsanitize=memory -fsanitize-memory-track-origins")
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

add_executable(day7 src/day7.cc)
target_compile_features(day7 PRIVATE cxx_std_17)

add_executable(day8 src/day8.cc)
target_compile_features(day8 PRIVATE cxx_std_17)

add_executable(day9 src/day9.cc)
target_compile_features(day9 PRIVATE cxx_std_17)

add_executable(day10 src/day10.cc)
target_compile_features(day10 PRIVATE cxx_std_17)

add_executable(day11 src/day11.cc)
target_compile_features(day11 PRIVATE cxx_std_17)

add_executable(day12 src/day12.cc)
target_compile_features(day12 PRIVATE cxx_std_17)

add_executable(day13 src/day13.cc)
target_compile_features(day13 PRIVATE cxx_std_17)

add_executable(day14 src/day14.cc)
target_compile_features(day14 PRIVATE cxx_std_17)

add_executable(day15 src/day15.cc)
target_compile_features(day15 PRIVATE cxx_std_17)

add_executable(day16 src/day16.cc)
target_compile_features(day16 PRIVATE cxx_std_17)

add_executable(day17 src/day17.cc)
target_compile_features(day17 PRIVATE cxx_std_17)

add_executable(day18 src/day18.cc)
target_compile_features(day18 PRIVATE cxx_std_17)

add_executable(day20 src/day20.cc)
target_compile_features(day20 PRIVATE cxx_std_17)

add_executable(day21 src/day21.cc)
target_compile_features(day21 PRIVATE cxx_std_17)

add_executable(day22 src/day22.cc)
target_compile_features(day22 PRIVATE cxx_std_17)

add_executable(day23 src/day23.cc)
target_compile_features(day23 PRIVATE cxx_std_17)

add_executable(day24 src/day24.cc)
target_compile_features(day24 PRIVATE cxx_std_17)
