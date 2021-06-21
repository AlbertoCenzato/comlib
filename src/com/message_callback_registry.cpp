#include "message_callback_registry.h"

namespace com {

void MessageCallbackRegistry::registerCallback(msg::MessageType type, Callback callback) { 
  CallbackRegistry* reg = getRegistry(type);
  if (!reg) {
    reg = &reserveNewRegistry(type);
  }
  reg->callback = callback;
}

void MessageCallbackRegistry::call(msg::MessageType type, const msg::IMessage& msg) const {
  const CallbackRegistry* reg = getRegistry(type);
  if (reg) {
    reg->callback(msg);
  }
}

bool MessageCallbackRegistry::isRegistered(msg::MessageType type) const {
  return getRegistry(type) != nullptr;
}

size_t MessageCallbackRegistry::registeredTypes() const {
  return registered_types;
}

const MessageCallbackRegistry::CallbackRegistry*
MessageCallbackRegistry::getRegistry(msg::MessageType type) const {
  for (const auto& reg : registry)
    if (reg.type == type)
      return &reg;
  return nullptr;
}

MessageCallbackRegistry::CallbackRegistry* 
MessageCallbackRegistry::getRegistry(msg::MessageType type) {
  return const_cast<CallbackRegistry*>(const_cast<const MessageCallbackRegistry*>(this)->getRegistry(type));
}

MessageCallbackRegistry::CallbackRegistry&
MessageCallbackRegistry::reserveNewRegistry(msg::MessageType type) {
  assert(registered_types < registry.size());  // TODO: handle register buffer exhaustion
  auto& reg = registry[registered_types++];
  reg.type = type;
  return reg;
}

}