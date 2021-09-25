#pragma once

enum MemoryLayout {
  AOS,
  SOA,
};

template <MemoryLayout layout, typename T1, typename T2>
class AOSOA {};