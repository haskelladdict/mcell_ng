cmake_minimum_required (VERSION 2.6)
project (mcell_ng)
add_executable(mcell_ng mcell_ng.cpp species.cpp molecules.cpp rng.cpp)

set(CMAKE_CXX_COMPILER "g++-5")
set(CMAKE_CXX_FLAGS "-O2 -std=c++11")
