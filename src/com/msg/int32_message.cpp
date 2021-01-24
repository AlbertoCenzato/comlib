#include "int32_message.h"
#include "../serialization.h"

namespace com::msg {

Int32Message::Int32Message(int32_t value) : value(value) {}

uint32_t Int32Message::getSize() const {
  return sizeof(value);
}

MessageType Int32Message::getMessageType() const {
  return type();
}

uint8_t* Int32Message::serialize(uint8_t* buffer) const {
  return com::serialize(value, buffer);
}

stdx::UPtr<IMessage> Int32Message::deserialize(const uint8_t* data, const uint8_t** new_buffer_ptr) {
  auto message = new msg::Int32Message();
  *new_buffer_ptr = com::deserialize(data, message->value);
  return message;
}

MessageType Int32Message::type() {
  return reg.message_type_id;
}


}