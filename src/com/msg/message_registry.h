#pragma once

#include "../stdx.h"
#include <cinttypes>
#include <string>

using namespace com;

namespace com::msg {

struct IMessage;

using MessageDeserializationCallback = stdx::UPtr<IMessage> (*)(const uint8_t* buffer, const uint8_t** new_buffer_ptr);

class MessageRegistry 
{
public:

  static const size_t MAP_SIZE = 256;

  static MessageRegistry& getInstance();

  uint16_t registerMessageDeserializationCallback(
    const std::string& message_type_id, 
    MessageDeserializationCallback callback);

  stdx::UPtr<IMessage> deserializeMessage(
    uint16_t message_type_id, 
    const uint8_t* buffer, 
    const uint8_t** new_buffer_ptr) const;

private:
  struct KeyValue {
    uint16_t message_type_id;
    MessageDeserializationCallback func;
  };

  KeyValue deserialization_map[MAP_SIZE];
  size_t next_available_empty_register;

  MessageRegistry();
};

}