#pragma once

#include "message_deserialization_registry.h"
#include "stdx.h"

namespace com {

class MessageCallbackRegistry
{
public:
  using Callback = void (*)(const msg::IMessage& msg);

  void registerCallback(msg::MessageType type, Callback callback);
  void call(msg::MessageType type, const msg::IMessage& msg) const;
  bool isRegistered(msg::MessageType type) const;
  size_t registeredTypes() const;

private:
  struct CallbackRegistry {
    msg::MessageType type = 0;
    Callback callback;
  };

  stdx::array<CallbackRegistry, 256> registry;
  size_t registered_types = 0;

  CallbackRegistry* getRegistry(msg::MessageType type);
  const CallbackRegistry* getRegistry(msg::MessageType type) const;
  CallbackRegistry& reserveNewRegistry(msg::MessageType type);
};


}
