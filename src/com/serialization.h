#pragma once

#include "utils.h"

namespace com {

template <class T>
uint8_t* serialize(const T& value, uint8_t* buffer) {
  utils::copyToLittleEndian(value, buffer);
  return buffer + sizeof(T);
}

template<class T>
const uint8_t* deserialize(const uint8_t* buffer, T& value) {
  value = utils::copyFromLittleEndian<T>(buffer);
  return buffer + sizeof(T);
}

}