#include "@|className|.h"
#include "../serialization.h"

namespace com::msg {

@|className|::@className(@constructorParams)
  @constructorParamsInit {}

uint32_t @|className|::getSize() const {
  return @sizeof;
}

MessageType @|className|::getMessageType() const {
  return type();
}

uint8_t* @|className|::serialize(uint8_t* buffer) const {
  @serialization
  return buffer;
}

stdx::UPtr<IMessage> @|className|::deserialize(const uint8_t* data, const uint8_t** new_buffer_ptr) {
  auto message = new msg::@|className|();
  @deserialization
  *new_buffer_ptr = data;
  return message;
}

MessageType @|className|::type() {
  return reg.message_type_id;
}

uint8_t* serialize(const @|className|& msg, uint8_t* buffer) {
  return msg.serialize(buffer);
}

}