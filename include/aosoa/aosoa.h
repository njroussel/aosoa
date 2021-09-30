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

template <MemoryLayout layout, std::size_t N, typename T1, typename Name1,
          typename T2, typename Name2>
struct AOSOA {
  static_assert(!(std::is_same<Name1, Name2>::value));
  void function() {}
  template <typename T, typename ParamName>
  T get(std::size_t index) {
    return T();
  }
};

template <std::size_t N, typename T1, typename Name1, typename T2,
          typename Name2>
struct AOSOA<ARRAY_OF_STRUCTURES, N, T1, Name1, T2, Name2> {
  void function() { std::cout << "SOA" << std::endl; }

  template <typename T, typename ParamName>
  T get(std::size_t index) {
    constexpr bool match1 =
        std::is_same<T, T1>::value && std::is_same<ParamName, Name1>::value;
    constexpr bool match2 =
        std::is_same<T, T2>::value && std::is_same<ParamName, Name2>::value;
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

  template <typename T, typename ParamName>
  T get(std::size_t index) {
    constexpr bool match1 =
        std::is_same<T, T1>::value && std::is_same<ParamName, Name1>::value;
    constexpr bool match2 =
        std::is_same<T, T2>::value && std::is_same<ParamName, Name2>::value;
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
template <MemoryLayout layout, typename NamedTuple>
struct NEW_AOSOA;

template <typename Type, typename Name, typename... Tail>
struct NEW_AOSOA<ARRAY_OF_STRUCTURES, NamedTuple<Type, Name, Tail...>> {
  using tuple_type = NamedTuple<Type, Name, Tail...>;

  void function() { std::cout << "SOA" << std::endl; }

  template <typename AttrType, typename AttrName>
  AttrType get(std::size_t index) {
    return m_arr[index].template get<AttrType, AttrName>();
  }

  tuple_type get(std::size_t index) { return m_arr[index]; }

 private:
  std::vector<tuple_type> m_arr;
};

template <typename Type, typename Name, typename... Tail>
struct NEW_AOSOA<STRUCTURE_OF_ARRAYS, NamedTuple<Type, Name, Tail...>> {
  using tuple_type = NamedTuple<Type, Name, Tail...>;

  void function() { std::cout << "AOS" << std::endl; }

  template <typename AttrType, typename AttrName>
  AttrType get(std::size_t index) {
    constexpr bool match = std::is_same<AttrType, Type>::value &&
                           std::is_same<AttrName, Name>::value;
    if constexpr (match) {
      return m_vec[index];
    } else {
      return m_tail.get(index);
    }
  }

 private:
  std::vector<Type> m_vec;
  NEW_AOSOA<STRUCTURE_OF_ARRAYS, NamedTuple<Tail...>> m_tail;
};

template <typename Type, typename Name>
struct NEW_AOSOA<STRUCTURE_OF_ARRAYS, NamedTuple<Type, Name>> {
  using tuple_type = NamedTuple<Type, Name>;

  void function() { std::cout << "AOS" << std::endl; }

  template <typename AttrType, typename AttrName>
  AttrType get(std::size_t index) {
    constexpr bool match = std::is_same<AttrType, Type>::value &&
                           std::is_same<AttrName, Name>::value;
    if constexpr (!match) {
      static_assert(
          match,
          "Template values do match the attribute type and attribute name!");
    } else {
      return m_vec[index];
    }
  }

 private:
  std::vector<Type> m_vec;
};
}  // namespace aosoa