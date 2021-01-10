/**
 * 
 * THIS should be an auto-generated file!!
 * 
 */

#pragma once

#include "../macros.h"

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
