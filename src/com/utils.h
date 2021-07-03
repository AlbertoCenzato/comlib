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

template <typename T>
struct Range {
  T min;
  T max;

  constexpr Range(T min, T max) : min(min), max(max) { assert(min <= max); }
};

template <class T>
bool isInRange(const T& value, const Range<T>& range_inclusive) {
  return range_inclusive.min <= value && value <= range_inclusive.max;
}

template <class T>
T hash(const String& str) {
  T hash = 5381;
  for (size_t i = 0; i < str.length(); ++i)
    hash = ((hash << 5) + hash) ^ str[i];  // (hash * 33) XOR c;    see http://www.cse.yorku.ca/~oz/hash.html

  return hash;
}

}  // namespace utils

}  // namespace com