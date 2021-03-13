#pragma once

#include "msg/message_registry.h"
#include <cassert>

namespace com {

class MessageCallbackRegistry
{
public:
  using Callback = void (*)(const msg::IMessage& msg);

  void registerCallback(msg::MessageType type, Callback callback);
  void call(msg::MessageType type, const msg::IMessage& msg) const;
  int getRegisterIndex(msg::MessageType type) const;
  bool isRegistered(msg::MessageType type) const;
  size_t registeredTypes() const;

private:
  struct CallbackRegistry {
    msg::MessageType type = 0;
    size_t registered_callbacks = 0;
    Callback callbacks[64];
  };

  CallbackRegistry registry[256];
  size_t registered_types = 0;

  CallbackRegistry* getRegistry(msg::MessageType type);
  const CallbackRegistry* getRegistry(msg::MessageType type) const;
  CallbackRegistry& reserveNewRegistry(msg::MessageType type);
};


}
