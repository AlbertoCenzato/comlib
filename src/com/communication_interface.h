#pragma once

#include "messages.h"

#ifdef ARDUINO_AVR_UNO
#include "String.h"
using String = String;
#else
#include <string>
using String = std::string;
#endif
//#include <ostream>

namespace com {

class IMessageSocket
{
public:
  virtual bool connect() = 0;
  virtual void disconnect() = 0;
  virtual bool send(const void* data, uint32_t bytes) = 0;
  virtual uint32_t receive(void* data, uint32_t bytes) = 0;
};

inline String to_string(MessageType message_type)
{
  switch (message_type)
  {
  case MessageType::BINARY_MESSAGE: return "BINARY_MESSAGE";
  case MessageType::EMPTY_MESSAGE:  return "EMPTY_MESSAGE";
  case MessageType::MOVE_MESSAGE: return "MOVE_MESSAGE";
  case MessageType::INT32_MESSAGE: return "INT32_MESSAGE";
  }

  return "UNKNOWN_VALUE";
}

}