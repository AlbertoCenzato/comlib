#include "communication.h"

namespace com {

namespace internal {


void fillMessage(BinaryMessage& message, const std::uint8_t* data);
void fillMessage(MoveMessage& message, const std::uint8_t* data);

void fillMessage(Message& message, MessageType type, const std::uint8_t* data) {
  message.type = type;
  switch (type)
  {
  case MessageType::BINARY_MESSAGE:
    fillMessage(message.message.binary, data);
    break;
  case MessageType::MOVE_MESSAGE:
    fillMessage(message.message.move, data);
  }
}

template <> inline
std::uint8_t* serialize<MessageType>(const MessageType& type, std::uint8_t* buffer) {
  return serialize(static_cast<std::int16_t>(type), buffer);
}

template <> inline 
std::uint8_t* serialize<float>(const float& value, std::uint8_t* buffer) {
  memcpy(buffer, &value, sizeof(float));
  return buffer + sizeof(float);
}


template<> inline 
const std::uint8_t* deserialize<float>(const std::uint8_t* data, float& value) {
  value = *reinterpret_cast<const float*>(data);
  return data + sizeof(float);
}

template<> inline 
const std::uint8_t* deserialize<MessageType>(const std::uint8_t* data, MessageType& value) {
  return deserialize(data, reinterpret_cast<std::int16_t&>(value));
}

void fillMessage(BinaryMessage& message, const std::uint8_t* data) {
  std::uint32_t payload_length;
  const std::uint8_t* payload = deserialize(data, payload_length);
  message.mallocAndSet(payload_length, payload);
}

void fillMessage(MoveMessage& message, const std::uint8_t* data) {
  data = deserialize(data, message.x);
  data = deserialize(data, message.y);
  deserialize(data, message.rot);
}


}


}