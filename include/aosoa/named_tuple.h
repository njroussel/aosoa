#pragma once
#include <array>
#include <iostream>
#include <type_traits>

namespace aosoa {
template <typename Type, typename Name, typename... Tail>
class NamedTuple {
  static_assert(!std::disjunction<std::is_same<Name, Tail>...>::value,
                "Type names must differ!");

 public:
  template <typename... TailTypes>
  NamedTuple(Type attr, TailTypes... tail) : m_attr(attr), m_tail(tail...) {}

  template <typename AttrType, typename AttrName>
  AttrType get() {
    constexpr bool match = std::is_same<AttrType, Type>::value &&
                           std::is_same<AttrName, Name>::value;
    if constexpr (match) {
      return m_attr;
    } else {
      return m_tail.template get<AttrType, AttrName>();
    }
  }

  NamedTuple<Tail...>& tail() { return m_tail; }

 private:
  Type m_attr;
  NamedTuple<Tail...> m_tail;
};

template <typename Type, typename Name>
class NamedTuple<Type, Name> {
 public:
  NamedTuple(Type attr) : m_attr(attr) {}

  template <typename AttrType, typename AttrName>
  AttrType get() {
    constexpr bool match = std::is_same<AttrType, Type>::value &&
                           std::is_same<AttrName, Name>::value;
    if constexpr (!match) {
      static_assert(
          match,
          "Template values do match the attribute type and attribute name!");
    } else {
      return m_attr;
    }
  }

 private:
  Type m_attr;
};
}  // namespace aosoa