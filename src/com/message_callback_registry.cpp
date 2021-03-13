#include "message_callback_registry.h"

namespace com {

void MessageCallbackRegistry::registerCallback(msg::MessageType type, Callback callback) { 
  CallbackRegistry* reg = getRegistry(type);
  if (!reg) {
    reg = &reserveNewRegistry(type);
  }
  reg->callbacks[reg->registered_callbacks++] = callback;
}

void MessageCallbackRegistry::call(msg::MessageType type, const msg::IMessage& msg) const {
  const CallbackRegistry* reg = getRegistry(type);
  if (reg) {
    for (size_t i = 0; i < reg->registered_callbacks; ++i) {
      Callback callback = reg->callbacks[i];
      callback(msg);
    }
  }
  else {
    // TODO: log something?
  }
}

int MessageCallbackRegistry::getRegisterIndex(msg::MessageType type) const {
  for (int i = 0; i < 256; ++i)  // TODO: remove hardcoded 256
    if (registry[i].type == type)
      return i;
  return -1;
}

bool MessageCallbackRegistry::isRegistered(msg::MessageType type) const {
  const int index = getRegisterIndex(type);
  return index >= 0;
}

size_t MessageCallbackRegistry::registeredTypes() const {
  return registered_types;
}

const MessageCallbackRegistry::CallbackRegistry*
MessageCallbackRegistry::getRegistry(msg::MessageType type) const {
  const int index = getRegisterIndex(type);
  if (index >= 0)
    return &registry[index];
  return nullptr;
}

MessageCallbackRegistry::CallbackRegistry* 
MessageCallbackRegistry::getRegistry(msg::MessageType type) {
  return const_cast<CallbackRegistry*>(const_cast<const MessageCallbackRegistry*>(this)->getRegistry(type));
}

MessageCallbackRegistry::CallbackRegistry&
MessageCallbackRegistry::reserveNewRegistry(msg::MessageType type) {
  assert(registered_types < 256);  // TODO: handle register buffer exhaustion
  auto& reg = registry[registered_types++];
  reg.type = type;
  return reg;
}

}