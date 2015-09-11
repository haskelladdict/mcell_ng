cmake_minimum_required (VERSION 2.6)
project (mcell_ng)

set(CMAKE_CXX_COMPILER "g++-5")
set(CMAKE_CXX_FLAGS "-O2 -Wall -Wextra -std=c++14")

add_subdirectory(src)
