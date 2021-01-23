#pragma once

#include "message_types.h"
#include "messages.h"
#include "../serialization.h"

namespace com::msg {

struct MoveMessage : public IMessage {
  float x;
  float y;
  float rot;

  MoveMessage() = default;
  MoveMessage(float x, float y, float rot);

  uint32_t getSize() const override;
  uint16_t getMessageType() const override;
  uint8_t* serialize(uint8_t* buffer) const override;

  static stdx::UPtr<IMessage> deserialize(const uint8_t* data, const uint8_t** new_data_ptr);

private:
  inline static DeserializationRegister<MoveMessage> reg{"MoveMessage"};
};

}
