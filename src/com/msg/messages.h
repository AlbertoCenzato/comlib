#pragma once

#include "empty_message.h"
#include "int32_message.h"
#include "move_message.h"

namespace com::msg {

struct Message {
  MessageType type;
  union {
    MoveMessage move;
    EmptyMessage empty;
    Int32Message int32;
  } message;
};

}
