#include "communication.h"
#include "serialization.h"

namespace com {

namespace internal {

void fillMessage(msg::Message& message, msg::MessageType type, const uint8_t* data) {
  message.type = type;
  switch (type)
  {
  case msg::MessageType::MOVE_MESSAGE:
    deserialize<msg::MoveMessage>(data, message.message.move);
    break;
  case msg::MessageType::EMPTY_MESSAGE:
    deserialize<msg::EmptyMessage>(data, message.message.empty);
    break;
  case msg::MessageType::INT32_MESSAGE:
    deserialize<msg::Int32Message>(data, message.message.int32);
    break;
  }
}

}


}