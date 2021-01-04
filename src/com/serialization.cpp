#include "serialization.h"
#include "messages.h"


namespace com {

template <> inline
uint8_t* serialize<MessageType>(const MessageType& type, uint8_t* buffer) {
  return serialize(static_cast<int16_t>(type), buffer);  // TODO(cenz): use an arduino-compatible std::underlying_type to avoid type errors
}

template <> inline
uint8_t* serialize<float>(const float& value, uint8_t* buffer) {
  memcpy(buffer, &value, sizeof(float));
  return buffer + sizeof(float);
}


template<> inline
const uint8_t* deserialize<float>(const uint8_t* data, float& value) {
  value = *reinterpret_cast<const float*>(data);
  return data + sizeof(float);
}

template<> inline
const uint8_t* deserialize<MessageType>(const uint8_t* data, MessageType& value) {
  auto data_ptr = deserialize(data, reinterpret_cast<int16_t&>(value));  // TODO(cenz): use an arduino-compatible std::underlying_type to avoid type errors
  if (utils::isInRange(value, MessageType::MOVE_MESSAGE, MessageType::INT32_MESSAGE))
    return data_ptr;
  return nullptr;
}

template<> inline
const uint8_t* deserialize<BinaryMessage>(const uint8_t* data, BinaryMessage& message) {
  uint32_t payload_length;
  const uint8_t* payload = deserialize(data, payload_length);
  message.mallocAndSet(payload_length, payload);
  return payload + payload_length;
}

template<> inline
const uint8_t* deserialize<MoveMessage>(const uint8_t* data, MoveMessage& message) {
  data = deserialize(data, message.x);
  data = deserialize(data, message.y);
  return deserialize(data, message.rot);
}

template<> inline
const uint8_t* deserialize<EmptyMessage>(const uint8_t* data, EmptyMessage& message) {
  return data;
}

template<> inline
const uint8_t* deserialize<Int32Message>(const uint8_t* data, Int32Message& message) {
  return deserialize(data, message.value);
}



}