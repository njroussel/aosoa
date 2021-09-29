#pragma once
#include <array>
#include <iostream>
#include <type_traits>

namespace aosoa {

template <typename T1, typename Name1, typename T2, typename Name2>
class NamedTuple2 {
  static_assert(!(std::is_same<Name1, Name2>::value));

 public:
  NamedTuple2(T1 name1, T2 name2) : m_name1(name1), m_name2(name2) {}

  template <typename T, typename Name>
  T get() {
    constexpr bool match1 =
        std::is_same<T, T1>::value && std::is_same<Name, Name1>::value;
    constexpr bool match2 =
        std::is_same<T, T2>::value && std::is_same<Name, Name2>::value;
    static_assert(match1 || match2);

    if constexpr (match1) {
      return m_name1;
    } else {
      return m_name2;
    }
  }

 private:
  T1 m_name1;
  T2 m_name2;
};
}  // namespace aosoa