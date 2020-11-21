#pragma once

#include <cstdint>
#include <cstddef>

enum class MessageType {
  ERROR = -1,
  ACK = 0,
  NACK = 1,
  PING = 2,
  LOG = 3,
  VELOCITY_VECTOR = 4
};

struct Message {
  MessageType type;
  std::size_t length;
  std::uint8_t* payload;
};
