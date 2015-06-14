cmake_minimum_required (VERSION 2.6)
project (mcell_ng)
add_executable(mcell_ng 
  geometry.cpp 
  io.cpp
  mcell_ng.cpp 
  molecules.cpp 
  rng.cpp 
  species.cpp 
  state.cpp
  util.cpp 
  io.cpp)

set(CMAKE_CXX_COMPILER "g++-5")
set(CMAKE_CXX_FLAGS "-O2 -Wall -Wextra -std=c++14")
