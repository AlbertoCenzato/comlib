/**
 * 
 * THIS should be an auto-generated file!!
 * 
 */

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

}


namespace com {

template <> inline
uint8_t* serialize<msg::MessageType>(const msg::MessageType& type, uint8_t* buffer) {
  return serialize(static_cast<int16_t>(type), buffer);  // TODO(cenz): use an arduino-compatible std::underlying_type to avoid type errors
}

template<> inline
const uint8_t* deserialize<msg::MessageType>(const uint8_t* data, msg::MessageType& value) {
  auto data_ptr = deserialize(data, reinterpret_cast<int16_t&>(value));  // TODO(cenz): use an arduino-compatible std::underlying_type to avoid type errors
  if (utils::isInRange(value, msg::MessageType::MOVE_MESSAGE, msg::MessageType::INT32_MESSAGE))
    return data_ptr;
  return nullptr;
}

}
