#  AOSOA

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

Array-of-structures/Structure-of-arrays:
```cpp
#include <aosoa/aosoa.h>
#include <aosoa/named_tuple.h>

// Define a named tuple which will replace the `struct` in a traditional array
// of structures (AOS) layout
struct CustomerId {};
struct Balance {};
using Customer = aosoa::NamedTuple<int, CustomerId, float, Balance>;

// Data layout can be changed in a single line by modifying the first
// template arugment
// using Customers = aosoa::AOSOA<aosoa::ARRAY_OF_STRUCTURES, Customer>;
using Customers = aosoa::AOSOA<aosoa::STRUCTURE_OF_ARRAYS, Customer>;

float findMaxBalance(Customers customers) {
  std::size_t size = customers.size();
  // The `size()` method returns either the size of the underlying arrays (SOA)
  // or the origial array (AOS).

  float maxBalance = 0;
  for (size_t i = 0; i < size; ++i) {
    float balance = customers.get<Balance>(i);
    // Attribute access is independent of data layout - this will not have
    // to be refactored if the `Customers` data layout is changed

    maxBalance = std::max(maxBalance, balance);
  }

  return maxBalance;
}

int main() {
  Customer c1(1, 2.7F);
  Customer c2(2, 1.0F);
  Customer c3(3, 8.5F);

  Customers customers;
  customers.push_back(c1);
  customers.push_back(c2);
  customers.push_back(c3);

  std::cout << findMaxBalance(customers) << std::endl;

  return EXIT_SUCCESS;
}

```
The demo file can be found [here](./src/aosoa_demo.cpp).