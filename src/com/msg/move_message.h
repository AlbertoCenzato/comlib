#pragma once

#include "message_types.h"

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
