#pragma once
#include <array>
#include <iostream>
#include <type_traits>

namespace aosoa {

template <typename T1, typename Name1, typename T2, typename Name2>
class NamedTuple2 {
  static_assert(!(std::is_same<Name1, Name2>::value),
                "Type names must differ!");

 public:
  NamedTuple2(T1 name1, T2 name2) : m_name1(name1), m_name2(name2) {}

  template <typename T, typename Name>
  T get() {
    constexpr bool match1 =
        std::is_same<T, T1>::value && std::is_same<Name, Name1>::value;
    constexpr bool match2 =
        std::is_same<T, T2>::value && std::is_same<Name, Name2>::value;
    static_assert(match1 || match2,
                  "Did not find a match for given type and type name!");

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

template <typename T, typename Name, typename... Ts>
class NamedTuple {
  static_assert(std::disjunction<std::is_same<T, Ts>...>::value,
                "Type names must differ!");

 public:
  NamedTuple(T name, Ts... tail) : m_name(name), m_tail(tail...) {}

  template <typename ParamType, typename ParamName>
  void get() {
    constexpr bool match = std::is_same<T, ParamName>::value &&
                           std::is_same<Name, ParamName>::value;
    if constexpr (match) {
      return m_name;
    } else {
      return m_tail.template get<ParamType, ParamName>();
    }
  }

 private:
  T m_name;
  NamedTuple<Ts...> m_tail;
};

template <typename T, typename Name>
class NamedTuple<T, Name> {
 public:
  NamedTuple(T name) : m_name(name) {}

  template <typename ParamType, typename ParamName>
  void get() {
    constexpr bool match = std::is_same<T, ParamName>::value &&
                           std::is_same<Name, ParamName>::value;
    if constexpr (!match) {
      static_assert(match, "Template values do match the type and type name!");
    }

    return m_name;
  }

 private:
  T m_name;
};
}  // namespace aosoa