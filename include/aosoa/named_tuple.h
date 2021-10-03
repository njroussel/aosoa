#pragma once
#include <array>
#include <iostream>
#include <type_traits>

namespace aosoa {

template <typename Type, typename Name, typename... Tail>
class NamedTuple;

template <typename AttrName, typename T>
struct named_tuple_attribute_type;

template <typename AttrName, typename Type, typename Name, typename... Tail>
struct named_tuple_attribute_type<AttrName, NamedTuple<Type, Name, Tail...>>
    : named_tuple_attribute_type<AttrName, NamedTuple<Tail...>> {
  using type = std::conditional_t<
      std::is_same<Name, AttrName>::value, Type,
      typename named_tuple_attribute_type<AttrName, NamedTuple<Tail...>>::type>;
};

template <typename AttrName, typename Type, typename Name>
struct named_tuple_attribute_type<AttrName, NamedTuple<Type, Name>> {
  using type = Type;
};

template <typename Type, typename Name, typename... Tail>
class NamedTuple {
  static_assert(!std::disjunction<std::is_same<Name, Tail>...>::value,
                "Type names must differ!");

 public:
  template <typename... TailTypes>
  NamedTuple(Type attr, TailTypes... tail) : m_attr(attr), m_tail(tail...) {}

  template <typename AttrName>
  typename named_tuple_attribute_type<AttrName,
                                      NamedTuple<Type, Name, Tail...>>::type&
  get() {
    constexpr bool match = std::is_same<AttrName, Name>::value;
    if constexpr (match) {
      return m_attr;
    } else {
      return m_tail.template get<AttrName>();
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

  template <typename AttrName>
  typename named_tuple_attribute_type<AttrName, NamedTuple<Type, Name>>::type&
  get() {
    constexpr bool match = std::is_same<AttrName, Name>::value;
    if constexpr (!match) {
      static_assert(match,
                    "Template argument dooes not match any attribute name!");
    } else {
      return m_attr;
    }
  }

 private:
  Type m_attr;
};
}  // namespace aosoa
