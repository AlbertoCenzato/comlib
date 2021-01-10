#pragma once

#include "message_types.h"
#include "../serialization.h"

namespace com::msg {

struct Int32Message {
  int32_t value;
};

constexpr uint32_t getSize(const Int32Message& message) {
  return sizeof(message.value);
}

template <>
constexpr MessageType getMessageType<Int32Message>() {
  return MessageType::INT32_MESSAGE;
}

}

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