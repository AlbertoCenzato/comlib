#include "serialization.h"

#include <string.h>

namespace com {

template <> inline
uint8_t* serialize<float>(const float& value, uint8_t* buffer) {
  memcpy(buffer, &value, sizeof(float));
  return buffer + sizeof(float);
}

template<> inline
const uint8_t* deserialize<float>(const uint8_t* data, float& value) {
  value = *reinterpret_cast<const float*>(data);
  return data + sizeof(float);
}

}