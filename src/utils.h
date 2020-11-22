#pragma once

#include <cstring>
#include <cinttypes>
#include "macros.h"

namespace com {

namespace utils {

constexpr bool isLittleEndian() {
  return LITTLE_ENDIAN;
}

void* reverseMemcpy(void* _restrict dest, const void* _restrict source, size_t len);

void littleEndianCopy(void* from, void* to, size_t len);
void bigEndianCopy(void* from, void* to, size_t len);

template <class T>
void copyToBytesLittleEndian(T value, void* dest) {
  littleEndianCopy(dest, reinterpret_cast<void*>(&value), sizeof(T));
}

template <class T>
void copyToBytesBigEndian(T value, void* dest) {
  bigEndianCopy(dest, reinterpret_cast<void*>(&value), sizeof(T));
}

template <class T>
T copyFromLittleEndianBytes(void* src) {
  T value;
  littleEndianCopy(&value, src, sizeof(T));
  return value;
}

template <class T>
T copyFromBigEndianBytes(void* src) {
  T value;
  bigEndianCopy(&value, src, sizeof(T));
  return value;
}

template <class T>
std::uint8_t* toUBytesArray(T& value) {
  return reinterpret_cast<std::uint8_t*>(&value);
}

template <class T>
T fromUBytesArray(std::uint8_t* data) {
  return *reinterpret_cast<T*>(data);
}


}  // namespace utils

}  // namespace com