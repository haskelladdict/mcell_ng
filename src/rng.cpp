// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#include "rng.hpp"


// constructor generating a uniform gaussian distribution
RngNorm::RngNorm(uint64_t seed) : seedVal_{seed}, mt_gen_{seed},
  rng_norm_{0.0, 1.0} {}
