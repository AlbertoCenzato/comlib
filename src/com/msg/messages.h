#pragma once

#include "../macros.h"
#include "../serialization.h"

#ifdef ARDUINO_AVR_UNO
#include "String.h"
using String = String;
#else
#include <string>
using String = std::string;
#endif

namespace com::msg {

enum class MessageType : int16_t {
  MOVE_MESSAGE,
  EMPTY_MESSAGE,
  INT32_MESSAGE
};

inline String to_string(MessageType message_type)
{
  switch (message_type)
  {
  case MessageType::EMPTY_MESSAGE:  return "EMPTY_MESSAGE";
  case MessageType::MOVE_MESSAGE: return "MOVE_MESSAGE";
  case MessageType::INT32_MESSAGE: return "INT32_MESSAGE";
  }

  return "UNKNOWN_VALUE";
}


template <class T>
constexpr MessageType getMessageType();


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


struct EmptyMessage {

};

constexpr uint32_t getSize(const EmptyMessage& message) {
  return uint32_t(0);
}

template <>
constexpr MessageType getMessageType<EmptyMessage>() {
  return MessageType::EMPTY_MESSAGE;
}

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

struct Message {
  MessageType type;
  union {
    MoveMessage move;
    EmptyMessage empty;
    Int32Message int32;
  } message;
};

}


namespace com {

template <> inline
uint8_t* serialize<msg::MessageType>(const msg::MessageType& type, uint8_t* buffer) {
  return serialize(static_cast<int16_t>(type), buffer);  // TODO(cenz): use an arduino-compatible std::underlying_type to avoid type errors
}

template <> inline
uint8_t* serialize<msg::MoveMessage>(const msg::MoveMessage& msg, uint8_t* buffer) {
  buffer = serialize(msg.x, buffer);
  buffer = serialize(msg.y, buffer);
  buffer = serialize(msg.rot, buffer);
  return buffer;
}

template <> inline 
uint8_t* serialize<msg::EmptyMessage>(const msg::EmptyMessage& msg, uint8_t* buffer) {
  return buffer;
}

template <> inline
uint8_t* serialize<msg::Int32Message>(const msg::Int32Message& msg, uint8_t* buffer) {
  return serialize(msg.value, buffer);
}

template<> inline
const uint8_t* deserialize<msg::MessageType>(const uint8_t* data, msg::MessageType& value) {
  auto data_ptr = deserialize(data, reinterpret_cast<int16_t&>(value));  // TODO(cenz): use an arduino-compatible std::underlying_type to avoid type errors
  if (utils::isInRange(value, msg::MessageType::MOVE_MESSAGE, msg::MessageType::INT32_MESSAGE))
    return data_ptr;
  return nullptr;
}

template<> inline
const uint8_t* deserialize<msg::MoveMessage>(const uint8_t* data, msg::MoveMessage& message) {
  data = deserialize(data, message.x);
  data = deserialize(data, message.y);
  return deserialize(data, message.rot);
}

template<> inline
const uint8_t* deserialize<msg::EmptyMessage>(const uint8_t* data, msg::EmptyMessage& message) {
  return data;
}

template<> inline
const uint8_t* deserialize<msg::Int32Message>(const uint8_t* data, msg::Int32Message& message) {
  return deserialize(data, message.value);
}

}

