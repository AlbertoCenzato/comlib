#include "serialization.h"
#include "msg/messages.h"

#include <string.h>

namespace com {

template <> inline
uint8_t* serialize<msg::MessageType>(const msg::MessageType& type, uint8_t* buffer) {
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