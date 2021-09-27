#pragma once
#include <array>
#include <iostream>
#include <type_traits>

namespace aosoa {
enum MemoryLayout {
  ARRAY_OF_STRUCTURES,
  STRUCTURE_OF_ARRAYS,
};

template <MemoryLayout layout, std::size_t N, typename T1, typename Name1,
          typename T2, typename Name2>
struct AOSOA {
  static_assert(!(std::is_same<Name1, Name2>::value));
  void function() {}
  template <typename T, typename Name>
  T get(std::size_t index) {
    return T();
  }
};

template <std::size_t N, typename T1, typename Name1, typename T2,
          typename Name2>
struct AOSOA<ARRAY_OF_STRUCTURES, N, T1, Name1, T2, Name2> {
  void function() { std::cout << "SOA" << std::endl; }

  template <typename T, typename Name>
  T get(std::size_t index) {
    constexpr bool match1 =
        std::is_same<T, T1>::value && std::is_same<Name, Name1>::value;
    constexpr bool match2 =
        std::is_same<T, T2>::value && std::is_same<Name, Name2>::value;
    static_assert(match1 || match2);

    if constexpr (match1) {
      return m_arr1.at(index);
    } else {
      return m_arr2.at(index);
    }
  }

 private:
  std::array<T1, N> m_arr1;
  std::array<T2, N> m_arr2;
};
template <std::size_t N, typename T1, typename Name1, typename T2,
          typename Name2>
struct AOSOA<STRUCTURE_OF_ARRAYS, N, T1, Name1, T2, Name2> {
  void function() { std::cout << "AOS" << std::endl; }

  template <typename T, typename Name>
  T get(std::size_t index) {
    constexpr bool match1 =
        std::is_same<T, T1>::value && std::is_same<Name, Name1>::value;
    constexpr bool match2 =
        std::is_same<T, T2>::value && std::is_same<Name, Name2>::value;
    static_assert(match1 || match2);

    if constexpr (match1) {
      return m_arr.at(index).t1;
    } else {
      return m_arr.at(index).t2;
    }
  }

 private:
  struct Struct {
    T1 t1;
    T2 t2;
  };

  std::array<Struct, N> m_arr;
};
}  // namespace aosoa