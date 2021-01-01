#include "communication.h"
#include "serialization.h"

namespace com {

namespace internal {

void fillMessage(Message& message, MessageType type, const std::uint8_t* data) {
  message.type = type;
  switch (type)
  {
  case MessageType::BINARY_MESSAGE:
    deserialize<BinaryMessage>(data, message.message.binary);
    break;
  case MessageType::MOVE_MESSAGE:
    deserialize<MoveMessage>(data, message.message.move);
    break;
  case MessageType::EMPTY_MESSAGE:
    deserialize<EmptyMessage>(data, message.message.empty);
    break;
  case MessageType::INT32_MESSAGE:
    deserialize<Int32Message>(data, message.message.int32);
    break;
  }
}

}


}