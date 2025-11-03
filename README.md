# simdmonte - A SIMD, concurrent Monte Carlo simulation engine 
## Contents
- [Overview](#overview)
- [Key Features](#key-features) 
- [Performance](#performance)
- [Design](#design)
- [Getting Started](#getting-started)
    * [Prerequisites](#prerequisites)
    * [Build](#build)
    * [Examples](#examples)
    * [Testing and Benchmarking](#testing-and-benchmarking)
- Future Work
- References and Credits

## Overview
simdmonte is a library for pricing vanilla and exotic options, currently supporting European and Asian options. The primary motivation for building this is to better understand the applications of SIMD and concurrency in C++. I also wanted to target exotic options - where simulation techniques are most suitable for pricing.
## Key Features
- Fully SIMD and concurrent path generation
- Custom SIMD random number generation 
    - Xorshift128+ for uniform generation
    - Box-Muller transform and Acklam's inverse CDF approximation for standard normal generation
- Modular and generic design to allow extensions of option types and underlying models

## Performance
All performance tests are carried out with `googlebenchmark`. See [benchmarking](#testing-and-benchmarking) for how to run the benchmarks on your machine
### Random Number Generation
Custom random standard normal generation showed a ~3.5x speedup over `std::mt19937` with `std::normal_distribution`, which takes 83.5ns to generate 8 values as opposed to 25.1ns from my implementation of Xorshift128+ and Box-Muller transform. My implementations have the added benefit of being SIMD-native, whereas STL methods would involve then loading the values to a SIMD register every call.

Standard uniform generation from Xorshift128+ proved to be the same speed as `std::mt19937` and `std::uniform_real_distribution`. Again, the benefit is that my implementation is SIMD native, and thus avoids having to load the results to a SIMD register each call.
### SIMD vs SISD pricing
## Design
## Getting Started
### Prerequisites
This library relies on AVX2 intrinsics, which are available on most modern CPUs.

To check if your CPU supports AVX2, run the following command:
```bash
lscpu | grep avx2
```
If no results show, your CPU does not support AVX2, and the library will not run.
### Build
```bash
# Clone the repository
git clone https://github.com/273dan/simdmonte
cd simdmonte

# Configure and compile
mkdir build
cmake -S . -B build
cmake --build build
```
### Examples
Examples are found in `examples/`, and will be compiled to `build/examples/`
### Testing and Benchmarking
Once built, to test, navigate to `build/` and run `ctest`. The tests cover the validity of custom RNG distributions and pricing accuracy.

To benchmark, run `build/benchmarks/benchmarks`. The benchmarks compare custom random number generation speed against methods from the C++ STL. In addition, simulation-per-second tests are run on the two option types over different simulation sizes.
## Future Work
- Implement additional option types (Barrier, American etc.)
- Support other simulation models (Heston, SABR etc.)





