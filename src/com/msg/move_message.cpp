#include "move_message.h"
#include "../serialization.h"

namespace com {

template <> inline
uint8_t* serialize<msg::MoveMessage>(const msg::MoveMessage& msg, uint8_t* buffer) {
  buffer = serialize(msg.x, buffer);
  buffer = serialize(msg.y, buffer);
  buffer = serialize(msg.rot, buffer);
  return buffer;
}

template<> inline
const uint8_t* deserialize<msg::MoveMessage>(const uint8_t* data, msg::MoveMessage& message) {
  data = deserialize(data, message.x);
  data = deserialize(data, message.y);
  return deserialize(data, message.rot);
}

}
