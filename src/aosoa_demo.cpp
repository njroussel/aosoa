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
