#pragma once

#include <cstring>
#include "macros.h"

namespace com {

namespace utils {

constexpr bool isLittleEndian() {
  return LITTLE_ENDIAN;
}

void* reverseMemcpy(void* _restrict dest, const void* _restrict source, size_t len);

template <class T>
void copyToBytesLittleEndian(T value, void* dest) {
  void* value_ptr = reinterpret_cast<void*>(&value);
  if _constexpr (isLittleEndian())
    memcpy(dest, value_ptr, sizeof(T));
  else
    reverseMemcpy(dest, value_ptr, sizeof(T));
}

template <class T>
void copyToBytesBigEndian(T value, void* dest) {
  void* value_ptr = reinterpret_cast<void*>(&value);
  if _constexpr (isLittleEndian())
    reverseMemcpy(dest, value_ptr, sizeof(T));
  else
    memcpy(dest, value_ptr, sizeof(T));
}

}  // namespace utils

}  // namespace com