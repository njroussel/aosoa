#include <aosoa/aosoa.h>
#include <aosoa/named_tuple.h>
#include <limits.h>

struct CustomerId {};
struct Balance {};
using Customer = aosoa::NamedTuple<int, CustomerId, float, Balance>;

// using Customers = aosoa::AOSOA<aosoa::ARRAY_OF_STRUCTURES, Customer>;
using Customers = aosoa::AOSOA<aosoa::STRUCTURE_OF_ARRAYS, Customer>;

int findMaxCustomerId(Customers customers) {
  std::size_t size = customers.size();
  int maxCustomerId = INT_MIN;
  for (size_t i = 0; i < size; ++i) {
    int customerId = customers.get<CustomerId>(i);
    maxCustomerId = std::max(maxCustomerId, customerId);
  }

  return maxCustomerId;
}

float averageBalance(Customers customers) {
  std::size_t size = customers.size();
  float sum = 0.0F;

  for (size_t i = 0; i < size; ++i) {
    float customerBalance = customers.get<Balance>(i);
    sum += customerBalance;
  }

  return sum / std::max(1.0F, (float)size);
}

int main() {
  Customer c1(1, 2.7F);
  Customer c2(2, 1.0F);
  Customer c3(3, 8.5F);

  Customers customers;
  customers.push_back(c1);
  customers.push_back(c2);
  customers.push_back(c3);

  // customers.other_get<Balance>(2);
  float x = c1.get<Balance>();

  std::cout << findMaxCustomerId(customers) << std::endl;
  std::cout << averageBalance(customers) << std::endl;

  return EXIT_SUCCESS;
}
