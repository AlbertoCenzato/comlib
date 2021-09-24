#include "com/message_deserialization_registry.h"
#include <string.h>
#include "com/i_message.h"

namespace com::msg {

MessageDeserializationRegistry::MessageDeserializationRegistry() : next_available_empty_register(0) {
  memset(deserialization_map.begin(), 0, sizeof(KeyValue)*deserialization_map.size());
}

MessageDeserializationRegistry& MessageDeserializationRegistry::getInstance() {
  static MessageDeserializationRegistry registry;
  return registry;
}

MessageType MessageDeserializationRegistry::registerMessageDeserializationCallback(
  const String& message_type_name,
  MessageDeserializationCallback callback)
{
  assert(next_available_empty_register < MAP_SIZE);

  const auto* pair = getKeyValuePair(message_type_name);
  assert(!pair && "message_type_name already registered");  // TODO: handle collisions

  const auto message_type_id = utils::hash<MessageType>(message_type_name);
  
  deserialization_map[next_available_empty_register].message_type_id = message_type_id;
  deserialization_map[next_available_empty_register].func = callback;
  next_available_empty_register++;

  return message_type_id;
}

stdx::UPtr<IMessage> MessageDeserializationRegistry::deserializeMessage(
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

const MessageDeserializationRegistry::KeyValue*
MessageDeserializationRegistry::getKeyValuePair(const String& message_type_name) const {
  const auto message_type_id = utils::hash<MessageType>(message_type_name);
  for (uint32_t i = 0; i < next_available_empty_register; ++i)
    if (deserialization_map[i].message_type_id == message_type_id)
      return &deserialization_map[i];
  return nullptr;
}

}