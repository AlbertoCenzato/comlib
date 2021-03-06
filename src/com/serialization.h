#pragma once

#include "utils.h"

namespace com {

template <class T>
uint8_t* serialize(const T& value, uint8_t* buffer) {
  utils::toLittleEndian(value, buffer);
  return buffer + sizeof(T);
}

template<class T>
const uint8_t* deserialize(const uint8_t* buffer, T& value) {
  value = utils::fromLittleEndian<T>(buffer);
  return buffer + sizeof(T);
}

}