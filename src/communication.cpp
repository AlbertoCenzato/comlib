#include "communication.h"
#include "utils.h"

namespace com {

namespace internal {


void fillMessage(BinaryMessage& message, const std::uint8_t* data);
void fillMessage(MoveMessage& message, const std::uint8_t* data);

void fillMessage(Message& message, MessageType type, const std::uint8_t* data) {
  message.type = type;
  switch (type)
  {
  case MessageType::BINARY_MESSAGE:
    fillMessage(message.message.binary, data);
    break;
  case MessageType::MOVE_MESSAGE:
    fillMessage(message.message.move, data);
  }
}

void fillMessage(BinaryMessage& message, const std::uint8_t* data) {
  size_t payload_length = utils::fromUBytesArray<std::size_t>(data);
  const std::uint8_t* payload = data + sizeof(std::size_t);
  message.mallocAndSet(payload_length, payload);
}

void fillMessage(MoveMessage& message, const std::uint8_t* data) {
  message.x = *reinterpret_cast<const float*>(data);
  message.y = *reinterpret_cast<const float*>(data + sizeof(float));
  message.rot = *reinterpret_cast<const float*>(data + 2 * sizeof(float));
}

}


}