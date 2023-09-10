# Low-Latency Programming Repository for High-Frequency Trading (HFT)

## Table of Contents
1. [Introduction](#introduction)
2. [Features](#features)
3. [Installation](#installation)
4. [Prerequisites](#prerequisites)
5. [Usage](#usage)
6. [Benchmarking](#benchmarking)
7. [Acknowledgments](#acknowledgments)

## Introduction

Welcome to the Low-Latency Programming Repository, a project aimed to serve both academics and industry practitioners in the realm of High-Frequency Trading (HFT). With a focus on optimizing latency-critical code, this repository provides comprehensive insights, techniques, design patterns, and best practices that are statistically benchmarked to mitigate latency in HFT systems.

## Features

- Comprehensive guide on low-latency programming techniques such as Cache Warming, Constexpr, Loop Unrolling, Lock-Free Programming, and Short-circuiting.
- Market-neutral statistical arbitrage pairs trading strategy optimized for low-latency.
- Implementation of the Disruptor pattern in C++ optimized for speed and scalability.
  
## Installation

Clone this repository to your local machine to get started.

```bash
git clone https://github.com/0burak/imperial_hft.git
cd imperial_hft
```

### Prerequisites

- C++ Compiler with C++11 support or higher
- [CMake](https://cmake.org/download/) version 3.10 or higher
- Google Benchmark

## Usage

After Google Benchmark has been installed and the benchmark folder if in the same directory as the mybenchmark.cc file, the user can compile the my-benchmark.cc file using the makefile provided in the repository.

```bash
g++ mybenchmark.cc -std=c++20 -isystem benchmark/include -Lbenchmark/build/src -lbenchmark -lpthread -o mybenchmark
```

## Benchmarking

All techniques and strategies are rigorously benchmarked for latency reduction, cache efficiency, and overall performance. The `benchmark/` directory contains all benchmarking scripts.

## Acknowledgments

- A special thanks to Dr Paul A. Bilokon for his evaluation and invaluable feedback.
