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
Custom random standard normal generation showed a ~3.5x speedup over `std::mt19937` with `std::normal_distribution`, which takes 83.5ns to generate 8 values as opposed to 25.1ns from my implementation of Xorshift128+ and Box-Muller transform. The STL values would also have to be then loaded into SIMD registers, further slowing it down.

Standard uniform generation from Xorshift128+ proved to be ~4x faster than `std::mt19937` and `std::uniform_real_distribution`. (3.2ns vs 13.4ns)
### Pricing
Pricing is benchmarked against a simple, SISD, single threaded implementation. The following metrics are simulations per second
| Option Type | SIMD, concurrent | SISD, single threaded | Speedup |
|-------------|------------------|-----------------------|---------|
| European    | 1.8 billion      | 72 million            | 25x     |
| Asian       | 11.2 million     | 300 thousand          | 37x     |

This is subject to change as pricing is further optimised
## Design
simdmonte is designed to be modular, allowing easy implementation of new option types and underlying models. Below is a quick summary before a brief of each key class.
### Summary
- `Pricer`: Main user-facing class. Contains the `price()` method which handles thread management and orchestrates the pricing loop.
- `Option`: An abstract base class holding generic data like `strike` and `expiry`. Derived classes hold variant-specific information. Each option is also a factory for its corresponding `Accumulator`
- `Accumulator`: An abstract interface for payoff calculation based on price path. Implementations of this interface handle the pricing logic for specific option variants
- `Underlying`: An abstract interface for a model representing movement of the underlying. Contains the `step()` method which generates new prices each step according to the specific model implementation.
- `Rng`: A dedicated SIMD random number generator, bypassing loading and storing from SIMD registers.
### Pricer
The `Pricer` class serves as a simple entry point for the user. It takes a `Params` struct in its constructor, which configures the simulation specific parameters such as the number of steps and simulations, and the specific models to use.

While `Params` is more specific to the simulation, `MarketData` is more specific to the option. This is why each call to `price()` takes both an `Option` and `MarketData`. The `MarketData` simply stores the current spot price, the volatility, and the risk free rate. The idea behind this design is that the top-level simulation paramaters are likely to be identical across the pricing of different options. So if a user wanted to price a series of options at different strikes, or under different market conditions, the same `Pricer` can be used.

### Option
The `Option` class is an abstract base class, only storing the `strike` and `expiry`. Derivations of `Option` may add additional fields, such as `avg_period` for `AsianOption` (the duration over which the average will be taken). Crucially, `Pricer::price()` takes the **base `Option` class**, so the same `Pricer` can be used across multiple option types, and new option types can be implemented more easily. 

This is achieved by the fact that each derived `Option` contains a factory method to return an `Accumulator` corresponding to that option's type. So the `Pricer` does not need to know anything about the Option in order to price it.
### Accumulator
`Accumulator` is an interface to handle payoff logic. In the main pricing loop, the `Accumulator` is updated with the current price. At the end of each simulation, the `Accumulator` returns the payoff for that simulation. 

Classes implementing `Accumulator` define the rules around payoff calculation. `update()` is called on each step regardless of option type, so path-dependent and path-independent options can share the same interface with minimal overhead. As `Accumulator` is created via a factory method from a derived `Option`, it can be constructed with all the relevant information from that option, all unbeknownst to the `Pricer`. So to implement a new option:
1. Define its extra fields (if any) in a child class of `Option`
2. Define its payoff logic with a class implementing `IAccumulator`
The reasoning behind this design choice was to separate option-specific logic from `Pricer`. 
### Underlying
`IUnderlying` is another interface which represents a stochastic model to generate price paths for the underlying asset. The interface only specifies two methods: `step()`, which generates the next price, and `set_current()`, which overrides the current price of the model to a given value.

Underlying models may need information about the option as well as the market, so the constructor takes a base `Option` as well as `Params` and `MarketData`. All this data, as well as the specific underlying model, is either contained in `Pricer` or passed to `price()`, so the `Pricer` can instantiate the `Underlying` throgh its factory. Although only one underlying model is currently implemented, this design allows "plug and play" for different underlying models, without changing logic **anywhere else**
### Rng
The `Rng` class is a fully SIMD implementation for standard normal and standard uniform number generation. The STL RNG functions served too slow for this project, and they have to be loaded into and unloaded from SIMD registers multiple times per step. Thus, `Rng` implements random number generation which happens entirely within SIMD registers. `state0` and `state1` together store 8 64-bit integers, using Xorshift128+ for PRNG each step. Through some bit manipulation, they can be transformed into standard uniform floats, which from there are passed to standard normal approximation functions.

Like many other parts of the library, normal generation is also modular. Two implementations currently exist, Box-Muller transform and Acklam's approximation function. The user may specify which method to use in `Params`. This is a bit pointless for now, as Box-Muller performs over 2x faster than Acklam's algorithm in this current implementation - however situations may arise in the future where one method may be slightly faster but is a slightly worse proxy for the normal distribution.

The vectorised `sin`, `cos`, `log` and `exp` functions are from [`avx_mathfun.h`](https://github.com/reyoung/avx_mathfun)
### Misc.
`price_types.h` contain two simple single-field struct wrappers for `__m256`. The sole purpose of these types are to distinguish whether a function recieves/returns a price in log space or price space. This has been very useful in debugging
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





