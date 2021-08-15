#pragma once

#include "message_deserialization_registry.h"
#include "stdx.h"

namespace com {

template <uint32_t REGISTRY_SIZE=256>
class MessageCallbackRegistry
{
public:
  using Callback = void (*)(const msg::IMessage& msg);

  void registerCallback(msg::MessageType type, Callback callback) {
    CallbackRegistry* reg = getRegistry(type);
    if (!reg) {
      reg = &reserveNewRegistry(type);
    }
    reg->callback = callback;
  }

  void call(msg::MessageType type, const msg::IMessage& msg) const {
    const CallbackRegistry* reg = getRegistry(type);
    if (reg) {
      reg->callback(msg);
    }
  }

  bool isRegistered(msg::MessageType type) const {
    return getRegistry(type) != nullptr;
  }

  uint32_t registeredTypes() const {
    return registered_types;
  }

private:
  struct CallbackRegistry {
    msg::MessageType type = 0;
    Callback callback;
  };

  stdx::array<CallbackRegistry, REGISTRY_SIZE> registry;
  uint32_t registered_types = 0;

  CallbackRegistry* getRegistry(msg::MessageType type) {
    return const_cast<CallbackRegistry*>(utils::asConst(this)->getRegistry(type));
  }

  const CallbackRegistry* getRegistry(msg::MessageType type) const {
    for (const auto& reg : registry)
      if (reg.type == type)
        return &reg;
    return nullptr;
  }

  CallbackRegistry& reserveNewRegistry(msg::MessageType type) {
    assert(registered_types < registry.size());  // TODO: handle register buffer exhaustion
    auto& reg = registry[registered_types++];
    reg.type = type;
    return reg;
  }

};


}
