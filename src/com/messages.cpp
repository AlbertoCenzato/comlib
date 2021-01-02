#include "messages.h"

#include <cstring>
#include <cstdlib>

namespace com {

void BinaryMessage::mallocAndSet(uint32_t payload_length, const uint8_t* payload) {
  this->payload_length = payload_length;
  freeMemory();
  this->payload = reinterpret_cast<uint8_t*>(std::malloc(payload_length));
  std::memcpy(this->payload, payload, payload_length);
}

void BinaryMessage::freeMemory() {
  if (payload)
    std::free(payload);
  payload = nullptr;
}


Message Message::buildEmptyMessage() {
  Message m;
  m.type = MessageType::BINARY_MESSAGE;
  m.message.binary.payload_length = 0;
  m.message.binary.payload = nullptr;
  return m;
}

}