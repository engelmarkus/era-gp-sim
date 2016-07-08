#ifndef ERAGPSIM_CORE_MEMORYVALUE_HPP_
#define ERAGPSIM_CORE_MEMORYVALUE_HPP_

#include <cstddef>

class MemoryValue {
public:
  MemoryValue();
  MemoryValue(MemoryValue &&) = default;
  MemoryValue &operator=(MemoryValue &&) = default;
  MemoryValue(const MemoryValue &) = default;
  MemoryValue &operator=(const MemoryValue &) = default;
  ~MemoryValue() = default;

  bool get(std::size_t) const;
  bool set(std::size_t, bool);
  void put(std::size_t, bool);
  std::size_t getByteSize() const;
  std::size_t getSize() const;
  std::size_t getByteCount() const;
};
#endif // ERAGPSIM_CORE_MEMORYVALUE_HPP_
