// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#ifndef RNG_HPP
#define RNG_HPP

#include <cstdint>
#include <random>


// Normal distributed random numbers using Mersenne-Twister (mt19937) as
// underlying random number source
class RngNorm {

public:
  RngNorm(uint64_t seed);

  double gen() {
    return rng_norm_(mt_gen_);
  }

private:

  uint64_t seedVal_;
  std::mt19937 mt_gen_;
  std::normal_distribution<double> rng_norm_;
};



#endif
