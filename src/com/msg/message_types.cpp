#include "message_types.h"
#include "../serialization.h"

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