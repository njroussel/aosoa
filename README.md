# AOSOA

Header-only C++ container to easily convert _array of structures_ (AOS) data layouts to _structure of arrays_ (SOA) layouts.

## Table of Contents

- [Features](#features)
- [Disclaimer](#disclaimer)
- [Demo](#demo)

## Features

- Cost-free abstraction (everything is resolved at compile-time)
- Container for named tuples
- Data layout modifications (usually) only require changing one line of code

## Disclaimer

This repository serves as a proof of concept for an AOS/SOA convertible container. It is lacking most features which would actually make it usable in a production environment (iterators, custom memory allocators, etc.).

## Demo

Named tuples:
```cpp
#include <aosoa/named_tuple.h>

// Define tuple element names by using empty structs
struct CustomerId {};
struct Balance {};

// Define named tuple, the templating order should be:
// [(type, name), (type, name), ...]
using Customer = aosoa::NamedTuple<int, CustomerId, float, Balance>;

int main() {
  Customer customer(1, 2.7F);

  // Access tuple elements through their name (type)
  // Element name resolution is done at compile-time. The naming of the tuple
  // elements is essentially free
  std::cout << customer.get<CustomerId>() << std::endl;
  std::cout << customer.get<Balance>() << std::endl;

  return EXIT_SUCCESS;
}
```
The demo file can be found [here](./src/named_tuple_demo.cpp).