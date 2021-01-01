#pragma once

#include <cstdint>

namespace com {

enum class MessageType {
  ERROR = -1,
  ACK = 0,
  NACK = 1,
  LOG = 2,
  MOVE_MESSAGE = 3,
  BINARY_MESSAGE = 4,
  EMPTY_MESSAGE = 5
};


struct BinaryMessage {
  static const MessageType type;

  std::uint32_t payload_length;
  std::uint8_t* payload;

  void mallocAndSet(std::uint32_t payload_length, const std::uint8_t* payload);
  void freeMemory();
};

struct MoveMessage {
  static const MessageType type;

  float x;
  float y;
  float rot;
};

struct EmptyMessage {
  static const MessageType type;
};

struct Message {
  MessageType type;
  union {
    BinaryMessage binary;
    MoveMessage move;
    EmptyMessage empty;
  } message;

  static Message buildEmptyMessage();  // TODO(cenz): rename
};

}