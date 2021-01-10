#pragma once

#include "message_types.h"
#include "../serialization.h"

namespace com::msg {

struct MoveMessage {
  float x;
  float y;
  float rot;
};

constexpr uint32_t getSize(const MoveMessage& message) {
  return sizeof(message.x) + sizeof(message.y) + sizeof(message.rot);
}

template <>
constexpr MessageType getMessageType<MoveMessage>() {
  return MessageType::MOVE_MESSAGE;
}

}


namespace com {

template <> inline
uint8_t* serialize<msg::MoveMessage>(const msg::MoveMessage& msg, uint8_t* buffer) {
  buffer = serialize(msg.x, buffer);
  buffer = serialize(msg.y, buffer);
  buffer = serialize(msg.rot, buffer);
  return buffer;
}

template<> inline
const uint8_t* deserialize<msg::MoveMessage>(const uint8_t* data, msg::MoveMessage& message) {
  data = deserialize(data, message.x);
  data = deserialize(data, message.y);
  return deserialize(data, message.rot);
}

}
