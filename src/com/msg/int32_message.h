#pragma once

#include "messages.h"
#include "../serialization.h"

namespace com::msg {

struct Int32Message : public IMessage {
  int32_t value;

  Int32Message() = default;
  Int32Message(int32_t value);

  uint32_t getSize() const override;
  MessageType getMessageType() const override;
  uint8_t* serialize(uint8_t* buffer) const override;

  static stdx::UPtr<IMessage> deserialize(const uint8_t* data, const uint8_t** new_buffer_ptr);

private:
  inline static DeserializationRegister<Int32Message> reg{"Int32Message"};
};

}
