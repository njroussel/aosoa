# AOSOA

Header-only C++ container to easily convert array of structures (AOS) data layouts to structure of arrays (SOA) layouts.

## Table of Contents

- [Features](#features)
- [Demo](#demo)

## Features

- Basic arithmetic operators
- Logic operators
- Implitcit type conversions
- Vector size is determined at compile-time
- Supports SSE4.2 and AVX2

## Demo

```cpp
// Declare pa(c)ked types - their width is determinded at compile-time to match
// the largest possible vector register size for the target architecture.
pfloat x = ...;

if any(x > 2.0f) {
  masked(x, x < 3.5f) = x * x; // Use masks to select specific lanes
}

// Store vectorized outputs to arrays
float buffer[pfloat::width];
store(&buffer, x);
```

A working implementation of the mandelbrot fractal is implemented in
[kr8md_demo.cpp](./src/kr8md_demo.cpp).
