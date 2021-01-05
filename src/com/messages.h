#pragma once

#include "macros.h"

namespace com {

enum class MessageType : int16_t {
  MOVE_MESSAGE,
  BINARY_MESSAGE,
  EMPTY_MESSAGE,
  INT32_MESSAGE
};

struct BinaryMessage {
  uint32_t payload_length;
  uint8_t* payload;

  void mallocAndSet(uint32_t payload_length, const uint8_t* payload);
  void freeMemory();

  uint32_t size() const;
};

struct MoveMessage {
  float x;
  float y;
  float rot;

  uint32_t size() const;
};

struct EmptyMessage {
  uint32_t size() const;
};

struct Int32Message {
  int32_t value;

  uint32_t size() const;
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