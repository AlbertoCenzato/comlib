#include "message_callback_registry.h"

namespace com {

void MessageCallbackRegistry::registerCallback(msg::MessageType type, Callback callback) {
  CallbackRegistry& reg = getRegistry(type);
  reg.callbacks[reg.registered_callbacks++] = callback;
}

void MessageCallbackRegistry::call(msg::MessageType type, const msg::IMessage& msg) const {
  const CallbackRegistry& reg = getRegistry(type);
  for (size_t i = 0; i < reg.registered_callbacks; ++i) {
    Callback callback = reg.callbacks[i];
    callback(msg);
  }
}

const MessageCallbackRegistry::CallbackRegistry& 
MessageCallbackRegistry::getRegistry(msg::MessageType type) const {
  size_t offset = 0;
  for (; registry[offset].type != 0 && offset < 256 && type != registry[offset].type; ++offset);

  assert(offset < 256);
  return registry[offset];
}

MessageCallbackRegistry::CallbackRegistry& 
MessageCallbackRegistry::getRegistry(msg::MessageType type) {
  return const_cast<CallbackRegistry&>(const_cast<const MessageCallbackRegistry*>(this)->getRegistry(type));
}


}