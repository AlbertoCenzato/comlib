#pragma once

#include "message_registry.h"
#include <cinttypes>

namespace com::msg {

struct IMessage {
  virtual uint32_t getSize() const = 0;
  virtual MessageType getMessageType() const = 0;
  virtual uint8_t* serialize(uint8_t* buffer) const = 0;

protected:
  template <class T>
  struct DeserializationRegister {
    MessageType message_type_id;

    DeserializationRegister(const std::string& class_name) {
      auto& reg = MessageRegistry::getInstance();
      message_type_id = reg.registerMessageDeserializationCallback(class_name, T::deserialize);
    }
  };
};

}
