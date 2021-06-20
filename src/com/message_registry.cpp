#include "message_registry.h"
#include "i_message.h"

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

  const int index = getKeyValueIndex(message_type_name);
  if (index >= 0)
    throw HashCollisionException{message_type_name + "already registered"};

  const auto message_type_id = utils::hash<MessageType>(message_type_name);
  
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

int MessageRegistry::getKeyValueIndex(const String& message_type_name) const {
  const auto message_type_id = utils::hash<MessageType>(message_type_name);
  for (int i = 0; i < next_available_empty_register; ++i)
    if (deserialization_map[i].message_type_id == message_type_id)
      return i;
  return -1;
}

}