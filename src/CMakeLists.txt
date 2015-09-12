find_package(Boost 1.58.0 REQUIRED)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  add_executable(mcell_ng 
    diffuse.cpp
    geometry.cpp 
    io.cpp
    mcell_ng.cpp 
    molecules.cpp 
    rng.cpp 
    species.cpp 
    state.cpp
    util.cpp)
endif()
