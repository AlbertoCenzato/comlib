#include "int32_message.h"
#include "../serialization.h"

namespace com {

template <> inline
uint8_t* serialize<msg::Int32Message>(const msg::Int32Message& msg, uint8_t* buffer) {
  return serialize(msg.value, buffer);
}

template<> inline
const uint8_t* deserialize<msg::Int32Message>(const uint8_t* data, msg::Int32Message& message) {
  return deserialize(data, message.value);
}

}