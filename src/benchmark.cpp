#include <aosoa/aosoa.h>
#include <aosoa/named_tuple.h>

#include <array>
#include <chrono>
#include <iostream>
#include <random>
#include <tuple>

const constexpr int N = 20000000;

const constexpr int SAMPLE_MAX = 10;
const constexpr int SAMPLE_MIN = 0;

struct MyInt {};
struct MyFloat {};
using MyNamedTuple = aosoa::NamedTuple<int, MyInt, float, MyFloat>;

// The function is templated here to reduce code duplication. In more
// conventional use cases, it should be possible to pass a `typedef` of some
// `aosoa::AOSOA` specialization.
template <aosoa::MemoryLayout layout>
int32_t averageInt(aosoa::AOSOA<layout, MyNamedTuple>& aosoa) {
  int size = (int)aosoa.size();
  int sum = 0;
  for (std::size_t i = 0; i < size; ++i) {
    sum += aosoa.template get<MyInt>(i);
  }

  size = std::max(1, size);
  return sum / size;
}

// Same remark as for `averageInt`: the function is exceptionnaly templated for
// the purpose of the benchmark
template <aosoa::MemoryLayout layout>
double averageFloat(aosoa::AOSOA<layout, MyNamedTuple>& aosoa) {
  int size = (int)aosoa.size();
  double sum = 0;
  for (std::size_t i = 0; i < size; ++i) {
    sum += aosoa.template get<MyFloat>(i);
  }

  size = std::max(1, size);
  return sum / (double)size;
}

int main() {
  aosoa::AOSOA<aosoa::ARRAY_OF_STRUCTURES, MyNamedTuple> aos;
  aosoa::AOSOA<aosoa::STRUCTURE_OF_ARRAYS, MyNamedTuple> soa;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distribInt(SAMPLE_MIN, SAMPLE_MAX);
  std::uniform_real_distribution<> distribFloat(SAMPLE_MIN, SAMPLE_MAX);
  for (int i = 0; i < N; ++i) {
    int sampleInt = distribInt(gen);
    float sampleFloat = (float)distribFloat(gen);

    MyNamedTuple tuple(sampleInt, sampleFloat);
    aos.push_back(tuple);
    soa.push_back(tuple);
  }

  auto t1 = std::chrono::high_resolution_clock::now();
  int avgAosInt = averageInt(aos);
  double avgAosFloat = averageFloat(aos);
  auto t2 = std::chrono::high_resolution_clock::now();
  auto durationAos =
      std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
  std::cout << "AverageAosInt: " << avgAosInt << std::endl;
  std::cout << "AverageAosFloat: " << avgAosFloat << std::endl;
  std::cout << "Duration: " << durationAos.count() << std::endl;

  t1 = std::chrono::high_resolution_clock::now();
  int avgSoaInt = averageInt(soa);
  double avgSoaFloat = averageFloat(soa);
  t2 = std::chrono::high_resolution_clock::now();
  auto durationSoa =
      std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
  std::cout << "AverageSoaInt: " << avgSoaInt << std::endl;
  std::cout << "AveragesSoaFloat: " << avgSoaFloat << std::endl;
  std::cout << "Duration: " << durationSoa.count() << std::endl;

  std::cout << "Done." << std::endl;
  return EXIT_SUCCESS;
}