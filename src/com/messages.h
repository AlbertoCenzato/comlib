#pragma once

#include <cstdint>

namespace com {

enum class MessageType : int16_t {
  ERROR = -1,
  ACK = 0,
  NACK = 1,
  LOG = 2,
  MOVE_MESSAGE = 3,
  BINARY_MESSAGE = 4,
  EMPTY_MESSAGE = 5,
  INT32_MESSAGE = 6
};


struct BinaryMessage {
  uint32_t payload_length;
  uint8_t* payload;

  void mallocAndSet(uint32_t payload_length, const uint8_t* payload);
  void freeMemory();
};

struct MoveMessage {
  float x;
  float y;
  float rot;
};

struct EmptyMessage {
};

struct Int32Message {
  int32_t value;
};

template <class T>
constexpr MessageType getMessageType();

template <>
constexpr MessageType getMessageType<BinaryMessage>() {
  return MessageType::BINARY_MESSAGE;
}

template <>
constexpr MessageType getMessageType<MoveMessage>() {
  return MessageType::MOVE_MESSAGE;
}

template <>
constexpr MessageType getMessageType<EmptyMessage>() {
  return MessageType::EMPTY_MESSAGE;
}

template <>
constexpr MessageType getMessageType<Int32Message>() {
  return MessageType::INT32_MESSAGE;
}

struct Message {
  MessageType type;
  union {
    BinaryMessage binary;
    MoveMessage move;
    EmptyMessage empty;
    Int32Message int32;
  } message;

  static Message buildEmptyMessage();  // TODO(cenz): rename
};

}