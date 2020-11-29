#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <ostream>

#include "messages.h"

namespace com {

class IMessageSocket
{
public:
  virtual bool connect() = 0;
  virtual void disconnect() = 0;
  virtual bool send(const void* data, size_t bytes) = 0;
  virtual size_t receive(void* data) = 0;
};

enum class ConnectionStatus {
  CONNECTING,
  CONNECTED,
  DISCONNECTED,
  ERROR
};


inline std::string to_string(MessageType message_type)
{
  switch (message_type)
  {
  case MessageType::ERROR: return "ERROR";
  case MessageType::ACK:   return "ACK";
  case MessageType::NACK:  return "NACK";
  case MessageType::EMPTY_MESSAGE:  return "EMPTY_MESSAGE";
  case MessageType::LOG:   return "LOG";
  case MessageType::MOVE_MESSAGE: return "MOVE_MESSAGE";
  }

  return "UNKNOWN_VALUE";
}

inline std::ostream& operator << (std::ostream& os, MessageType message_type)
{
  os << to_string(message_type);
  return os;
}

}