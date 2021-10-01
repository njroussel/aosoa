#pragma once
#include <aosoa/named_tuple.h>

#include <array>
#include <iostream>
#include <type_traits>
#include <vector>

namespace aosoa {
enum MemoryLayout {
  ARRAY_OF_STRUCTURES,
  STRUCTURE_OF_ARRAYS,
};
template <MemoryLayout layout, typename NamedTuple>
struct AOSOA;

template <typename Type, typename Name, typename... Tail>
struct AOSOA<ARRAY_OF_STRUCTURES, NamedTuple<Type, Name, Tail...>> {
  using tuple_type = NamedTuple<Type, Name, Tail...>;

  void function() { std::cout << "SOA" << std::endl; }

  std::size_t size() { return m_vec.size(); }

  template <typename AttrName>
  typename named_tuple_attribute_type<AttrName, tuple_type>::type& get(
      std::size_t index) {
    return m_vec[index].template get<AttrName>();
  }

  void push_back(NamedTuple<Type, Name, Tail...>& tuple) {
    m_vec.push_back(tuple);
  }

 private:
  std::vector<tuple_type> m_vec;
};

template <typename Type, typename Name, typename... Tail>
struct AOSOA<STRUCTURE_OF_ARRAYS, NamedTuple<Type, Name, Tail...>> {
  using tuple_type = NamedTuple<Type, Name, Tail...>;

  void function() { std::cout << "AOS" << std::endl; }

  std::size_t size() { return m_vec.size(); }

  template <typename AttrName>
  typename named_tuple_attribute_type<AttrName, tuple_type>::type& get(
      std::size_t index) {
    constexpr bool match = std::is_same<AttrName, Name>::value;
    if constexpr (match) {
      return m_vec[index];
    } else {
      return m_tail.template get<AttrName>(index);
    }
  }

  void push_back(NamedTuple<Type, Name, Tail...>& tuple) {
    m_vec.push_back(tuple.template get<Name>());
    m_tail.push_back(tuple.tail());
  }

 private:
  std::vector<Type> m_vec;
  AOSOA<STRUCTURE_OF_ARRAYS, NamedTuple<Tail...>> m_tail;
};

template <typename Type, typename Name>
struct AOSOA<STRUCTURE_OF_ARRAYS, NamedTuple<Type, Name>> {
  using tuple_type = NamedTuple<Type, Name>;

  void function() { std::cout << "AOS" << std::endl; }

  std::size_t size() { return m_vec.size(); }

  template <typename AttrName>
  Type& get(std::size_t index) {
    constexpr bool match = std::is_same<AttrName, Name>::value;
    if constexpr (!match) {
      static_assert(match, "Template value does not match the attribute name!");
    } else {
      return m_vec[index];
    }
  }

  void push_back(NamedTuple<Type, Name>& tuple) {
    m_vec.push_back(tuple.template get<Name>());
  }

 private:
  std::vector<Type> m_vec;
};
}  // namespace aosoa