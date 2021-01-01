#pragma once

#include "utils.h"

namespace com {

template <class T>
std::uint8_t* serialize(const T& value, std::uint8_t* buffer) {
  utils::copyToLittleEndian(value, buffer);
  return buffer + sizeof(T);
}

template<class T>
const std::uint8_t* deserialize(const std::uint8_t* buffer, T& value) {
  value = utils::copyFromLittleEndian<T>(buffer);
  return buffer + sizeof(T);
}

}