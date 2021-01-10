#pragma once

#include "empty_message.h"
#include "int32_message.h"
#include "move_message.h"

namespace com::msg {

struct Message {
  MessageType type;
  union {
    MoveMessage move_message;
    EmptyMessage empty_message;
    Int32Message int32_message;
  } message;
};

void fillMessage(Message& message, MessageType type, const uint8_t* data);

}
