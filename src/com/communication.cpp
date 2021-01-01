#include "communication.h"

namespace com {

namespace internal {


void fillMessage(BinaryMessage& message, const std::uint8_t* data);
void fillMessage(MoveMessage& message, const std::uint8_t* data);
void fillMessage(EmptyMessage& message, const std::uint8_t* data);

void fillMessage(Message& message, MessageType type, const std::uint8_t* data) {
  message.type = type;
  switch (type)
  {
  case MessageType::BINARY_MESSAGE:
    fillMessage(message.message.binary, data);
    break;
  case MessageType::MOVE_MESSAGE:
    fillMessage(message.message.move, data);
    break;
  case MessageType::EMPTY_MESSAGE:
    fillMessage(message.message.empty, data);
  }
}

void fillMessage(BinaryMessage& message, const std::uint8_t* data) {
  std::uint32_t payload_length;
  const std::uint8_t* payload = utils::deserialize(data, payload_length);
  message.mallocAndSet(payload_length, payload);
}

void fillMessage(MoveMessage& message, const std::uint8_t* data) {
  data = utils::deserialize(data, message.x);
  data = utils::deserialize(data, message.y);
  utils::deserialize(data, message.rot);
}

void fillMessage(EmptyMessage& message, const std::uint8_t* data) {

}

}


}