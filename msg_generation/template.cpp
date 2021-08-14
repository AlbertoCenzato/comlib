#include "com/msg/@|className|.h"

namespace com::msg {

@|className|::@className(@constructorParams)
  @constructorParamsInit {}

uint32_t @|className|::getSize() const {
  uint32_t size = 0;
  @sizeof
  return size;
}

MessageType @|className|::getMessageType() const {
  return type();
}

uint8_t* @|className|::serialize(uint8_t* buffer) const {
  return ::com::serialize(*this, buffer);
}

stdx::UPtr<IMessage> @|className|::deserialize(const uint8_t* data, const uint8_t** new_buffer_ptr) {
  auto message = new msg::@|className|();
  *new_buffer_ptr = ::com::deserialize(data, *message);
  return message;
}

MessageType @|className|::type() {
  return reg.message_type_id;
}

}


namespace com {

template <>
uint8_t* serialize<msg::@|className|>(const msg::@|className|& msg, uint8_t* buffer) {
  @serialization
  return buffer;
}

template <>
const uint8_t* deserialize<msg::@|className|>(const uint8_t* buffer, msg::@|className|& msg) {
  @deserialization
  return buffer;
}

}
