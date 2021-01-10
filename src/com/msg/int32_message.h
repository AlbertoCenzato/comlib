#pragma once

#include "message_types.h"

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
