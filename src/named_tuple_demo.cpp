#include <aosoa/named_tuple.h>
#include <limits.h>

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
