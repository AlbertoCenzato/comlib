#include "message_registry.h"
#include "messages.h"
#include <cassert>

namespace com::msg {

MessageRegistry::MessageRegistry() : next_available_empty_register(0) {
  memset(deserialization_map, 0, sizeof(deserialization_map));
}

MessageRegistry& MessageRegistry::getInstance() {
  static MessageRegistry registry;
  return registry;
}

MessageType MessageRegistry::registerMessageDeserializationCallback(
  const String& message_type_name,
  MessageDeserializationCallback callback)
{
  assert(next_available_empty_register < MAP_SIZE);

  // TODO: fix this horrible static_cast
  MessageType message_type_id = static_cast<MessageType>(std::hash<std::string>{}(message_type_name));  // TODO: provide an arduino-compatible hash function that hashes from string to uint16_t

  deserialization_map[next_available_empty_register].message_type_id = message_type_id;
  deserialization_map[next_available_empty_register].func = callback;
  next_available_empty_register++;

  return message_type_id;
}

stdx::UPtr<IMessage> MessageRegistry::deserializeMessage(
  MessageType message_type_id, 
  const uint8_t* buffer, 
  const uint8_t** new_buffer_ptr) const 
{
  MessageDeserializationCallback callback = nullptr;
  for (size_t i = 0; i < next_available_empty_register && callback == nullptr; i++) {
    if (deserialization_map[i].message_type_id == message_type_id)
      callback = deserialization_map[i].func;
  }

  if (callback)
    return callback(buffer, new_buffer_ptr);

  return nullptr;
}

}