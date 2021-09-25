#include <aosoa/aosoa.h>

#include <array>
#include <chrono>
#include <iostream>
#include <random>
#include <tuple>

const constexpr int N = 200000;

const constexpr int POS_MAX = 10;
const constexpr int POS_MIN = 0;

struct Obj {
  int32_t position{};
  float value{};
};

struct SoaObj {
  std::array<int32_t, N> position{};
  std::array<float, N> value{};
};

int32_t averagePositionAOS(std::array<Obj, N>& arr) {
  return std::accumulate(arr.begin(), arr.end(), 0,
                         [](int sum, Obj& obj) { return sum + obj.position; }) /
         N;
}
int32_t averagePositionSOA(SoaObj& soa) {
  return std::accumulate(soa.position.begin(), soa.position.end(), 0) / N;
}

int main() {
  AOSOA<AOS, int, float> meme;

  //
  std::array<Obj, N> aos;
  SoaObj soa;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(POS_MIN, POS_MAX);
  for (int i = 0; i < N; ++i) {
    int sample = distrib(gen);
    aos.at(i).position = sample;
    soa.position.at(i) = sample;
  }

  auto t1 = std::chrono::high_resolution_clock::now();
  int avgAOS = averagePositionAOS(aos);
  auto t2 = std::chrono::high_resolution_clock::now();
  auto durationAos =
      std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
  std::cout << "AverageAOS: " << avgAOS << std::endl;
  std::cout << "Duration: " << durationAos.count() << std::endl;

  t1 = std::chrono::high_resolution_clock::now();
  int avgSOA = averagePositionSOA(soa);
  t2 = std::chrono::high_resolution_clock::now();
  auto durationSoa =
      std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
  std::cout << "AverageSOA: " << avgSOA << std::endl;
  std::cout << "Duration: " << durationSoa.count() << std::endl;

  std::cout << "Done." << std::endl;
  return EXIT_SUCCESS;
}