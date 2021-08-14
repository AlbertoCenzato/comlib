#pragma once

#include "utils.h"
#include "stdx.h"

namespace com {

template <class T>
uint8_t* serialize(const T& value, uint8_t* buffer) {
  utils::toLittleEndian(value, buffer);
  return buffer + sizeof(T);
}

template <typename T, size_t size>
uint8_t* serialize(const stdx::array<T, size>& array, uint8_t* buffer) {
  for (const auto& value : array) 
    buffer = serialize(value, buffer);
  return buffer;
}

template<class T>
const uint8_t* deserialize(const uint8_t* buffer, T& value) {
  value = utils::fromLittleEndian<T>(buffer);
  return buffer + sizeof(T);
}

template <typename T, size_t size>
const uint8_t* deserialize(const uint8_t* buffer, stdx::array<T, size>& array) {
  for (auto& value : array)
    buffer = deserialize(buffer, value);
  return buffer;
}

}