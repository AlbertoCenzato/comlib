#pragma once

#include "stdx.h"

using namespace com;

namespace com::msg {

using MessageType = uint16_t;

struct IMessage;

using MessageDeserializationCallback = stdx::UPtr<IMessage> (*)(const uint8_t* buffer, const uint8_t** new_buffer_ptr);

struct HashCollisionException {
  String message;
};

class MessageDeserializationRegistry   // TODO: constexpr?
{
public:

  static const size_t MAP_SIZE = 256;

  static MessageDeserializationRegistry& getInstance();

  MessageType registerMessageDeserializationCallback(
    const String& message_type_id, 
    MessageDeserializationCallback callback);

  stdx::UPtr<IMessage> deserializeMessage(
    MessageType message_type_id, 
    const uint8_t* buffer, 
    const uint8_t** new_buffer_ptr) const;

private:
  struct KeyValue {
    MessageType message_type_id;
    MessageDeserializationCallback func;
  };

  stdx::array<KeyValue, MAP_SIZE> deserialization_map;
  size_t next_available_empty_register;

  MessageDeserializationRegistry();

  const KeyValue* getKeyValuePair(const String& message_type_id) const;
};

}