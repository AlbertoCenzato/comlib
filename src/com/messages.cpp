#include "messages.h"

#include <string.h>
#include <stdlib.h>

namespace com {

uint32_t MoveMessage::size() const {
  return sizeof(x) + sizeof(y) + sizeof(rot);
}

uint32_t EmptyMessage::size() const {
  return uint32_t(0);
}

uint32_t Int32Message::size() const {
  return sizeof(value);
}

}