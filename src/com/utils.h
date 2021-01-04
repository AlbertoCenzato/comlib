#pragma once

#include "macros.h"

#include <assert.h>

namespace com {

namespace utils {

constexpr bool isLittleEndian() {
  return LITTLE_ENDIAN;
}

void* reverseMemcpy(void* _restrict dest, const void* _restrict source, size_t len);

void littleEndianCopy(void* dest, const void* src, size_t len);
void bigEndianCopy(void* dest, const void* src, size_t len);

template <class T>
void toLittleEndian(T value, void* dest) {
  littleEndianCopy(dest, reinterpret_cast<const void*>(&value), sizeof(T));
}

template <class T>
void toBigEndian(T value, void* dest) {
  bigEndianCopy(dest, reinterpret_cast<const void*>(&value), sizeof(T));
}

template <class T>
T fromLittleEndian(const void* src) {
  T value;
  littleEndianCopy(&value, src, sizeof(T));
  return value;
}

template <class T>
T fromBigEndian(const void* src) {
  T value;
  bigEndianCopy(&value, src, sizeof(T));
  return value;
}

template <class T>
uint8_t* asUBytesArray(T& value) {
  return reinterpret_cast<uint8_t*>(&value);
}

template <class T>
const uint8_t* asUBytesArray(const T& value) {
  return reinterpret_cast<const uint8_t*>(&value);
}

template <class T>
T fromUBytesArray(const uint8_t* data) {
  return *reinterpret_cast<const T*>(data);
}

void profileTime(const char* name, void(*func)());

void shiftBytesLeft(uint8_t* array, size_t length, size_t shift);
void shiftBytesRight(uint8_t* array, size_t length, size_t shift);

template <class T>
bool isInRange(const T& value, const T& min, const T& max) {
  assert(min <= max);
  return min <= value && value <= max;
}

}  // namespace utils

}  // namespace com