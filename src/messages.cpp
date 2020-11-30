#include "messages.h"

#include <cstring>


namespace com {

const MessageType BinaryMessage::type = MessageType::BINARY_MESSAGE;

void BinaryMessage::mallocAndSet(std::uint32_t payload_length, const std::uint8_t* payload) {
  this->payload_length = payload_length;
  freeMemory();
  this->payload = reinterpret_cast<std::uint8_t*>(malloc(payload_length));
  std::memcpy(this->payload, payload, payload_length);
}

void BinaryMessage::freeMemory() {
  if (payload)
    free(payload);
  payload = nullptr;
}


const MessageType MoveMessage::type = MessageType::MOVE_MESSAGE;
const MessageType EmptyMessage::type = MessageType::EMPTY_MESSAGE;


Message Message::buildEmptyMessage() {
  Message m;
  m.type = MessageType::BINARY_MESSAGE;
  m.message.binary.payload_length = 0;
  m.message.binary.payload = nullptr;
  return m;
}

}