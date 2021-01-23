#include "move_message.h"
#include "../serialization.h"

namespace com::msg {

MoveMessage::MoveMessage(float x, float y, float rot) : x(x), y(y), rot(rot) {}

uint32_t MoveMessage::getSize() const {
  return sizeof(x) + sizeof(y) + sizeof(rot);
}

MessageType MoveMessage::getMessageType() const {
  return reg.message_type_id;
}

uint8_t* MoveMessage::serialize(uint8_t* buffer) const {
  buffer = com::serialize(x, buffer);
  buffer = com::serialize(y, buffer);
  buffer = com::serialize(rot, buffer);
  return buffer;
}

stdx::UPtr<IMessage> MoveMessage::deserialize(const uint8_t* data, const uint8_t** new_data_ptr) {
  auto message = new MoveMessage{}; 
  data = com::deserialize(data, message->x);
  data = com::deserialize(data, message->y);
  *new_data_ptr = com::deserialize(data, message->rot);
  return message;  // TODO: variance of return types?
}

}
