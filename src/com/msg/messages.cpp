#include "messages.h"
#include "../serialization.h"

namespace com::msg {

void fillMessage(Message& message, MessageType type, const uint8_t* data) {
  message.type = type;
  switch (type)
  {
  case MessageType::MOVE_MESSAGE:
    deserialize<MoveMessage>(data, message.message.move_message);
    break;
  case MessageType::EMPTY_MESSAGE:
    deserialize<EmptyMessage>(data, message.message.empty_message);
    break;
  case MessageType::INT32_MESSAGE:
    deserialize<Int32Message>(data, message.message.int32_message);
    break;
  }
}

}