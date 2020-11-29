#pragma once

#include <cstring>
#include <cinttypes>
#include <functional>
#include <chrono>
#include <iostream>
#include "macros.h"

namespace com {

namespace utils {

constexpr bool isLittleEndian() {
  return LITTLE_ENDIAN;
}

void* reverseMemcpy(void* _restrict dest, const void* _restrict source, size_t len);

void littleEndianCopy(void* dest, const void* src, size_t len);
void bigEndianCopy(void* dest, const void* src, size_t len);

template <class T>
void copyToLittleEndian(T value, void* dest) {
  littleEndianCopy(dest, reinterpret_cast<const void*>(&value), sizeof(T));
}

template <class T>
void copyToBigEndian(T value, void* dest) {
  bigEndianCopy(dest, reinterpret_cast<const void*>(&value), sizeof(T));
}

template <class T>
T copyFromLittleEndian(const void* src) {
  T value;
  littleEndianCopy(&value, src, sizeof(T));
  return value;
}

template <class T>
T copyFromBigEndian(const void* src) {
  T value;
  bigEndianCopy(&value, src, sizeof(T));
  return value;
}


template <class T>
std::uint8_t* toUBytesArray(T& value) {
  return reinterpret_cast<std::uint8_t*>(&value);
}

template <class T>
const std::uint8_t* toUBytesArray(const T& value) {
  return reinterpret_cast<const std::uint8_t*>(&value);
}

template <class T>
T fromUBytesArray(const std::uint8_t* data) {
  return *reinterpret_cast<const T*>(data);
}

void profileTime(const char* name, std::function<void()> func);

}  // namespace utils

}  // namespace com