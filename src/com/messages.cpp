#include "messages.h"

#include <string.h>
#include <stdlib.h>

namespace com {

void BinaryMessage::mallocAndSet(uint32_t payload_length, const uint8_t* payload) {
  this->payload_length = payload_length;
  freeMemory();
  this->payload = reinterpret_cast<uint8_t*>(malloc(payload_length));
  memcpy(this->payload, payload, payload_length);
}

void BinaryMessage::freeMemory() {
  if (payload)
    free(payload);
  payload = nullptr;
}

uint32_t BinaryMessage::size() const {
  return sizeof(payload_length) + payload_length;
}

uint32_t MoveMessage::size() const {
  return sizeof(x) + sizeof(y) + sizeof(rot);
}

uint32_t EmptyMessage::size() const {
  return uint32_t(0);
}

uint32_t Int32Message::size() const {
  return sizeof(value);
}



Message Message::buildEmptyMessage() {
  Message m;
  m.type = MessageType::BINARY_MESSAGE;
  m.message.binary.payload_length = 0;
  m.message.binary.payload = nullptr;
  return m;
}

}