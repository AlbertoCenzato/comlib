#include "serialization.h"
#include "messages.h"


namespace com {

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

template<> inline
const uint8_t* deserialize<BinaryMessage>(const std::uint8_t* data, BinaryMessage& message) {
  std::uint32_t payload_length;
  const std::uint8_t* payload = deserialize(data, payload_length);
  message.mallocAndSet(payload_length, payload);
  return payload + payload_length;
}

template<> inline
const uint8_t* deserialize<MoveMessage>(const std::uint8_t* data, MoveMessage& message) {
  data = deserialize(data, message.x);
  data = deserialize(data, message.y);
  return deserialize(data, message.rot);
}

template<> inline
const uint8_t* deserialize<EmptyMessage>(const std::uint8_t* data, EmptyMessage& message) {
  return data;
}

template<> inline
const uint8_t* deserialize<Int32Message>(const std::uint8_t* data, Int32Message& message) {
  return deserialize(data, message.value);
}



}