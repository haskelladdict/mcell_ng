cmake_minimum_required (VERSION 2.6)
project (mcell_ng)

set(CMAKE_CXX_COMPILER "g++-5")
# NOTE: the -Wno-deprecated-declarations is due to boost's use of auto_ptr which
# are deprecated in C++11 and the resulting warnings. It should be removed once
# boost has been "fixed".
set(CMAKE_CXX_FLAGS "-O2 -Wall -Wextra -std=c++14 -Wno-deprecated-declarations")

add_subdirectory(src)
