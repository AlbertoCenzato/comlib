#pragma once

#include "communication_interface.h"
#include "stream_message_reader.h"
#include "serialization.h"
#include "msg/messages.h"
#include "msg/message_registry.h"
#include "msg/message_types.h"

namespace com {

struct Message;
void swap(Message& a, Message& b);

struct Message {
  uint16_t message_type_id;
  stdx::UPtr<msg::IMessage> message;

  Message() : message_type_id(0), message() {}
  Message(uint16_t message_type_id, stdx::UPtr<msg::IMessage>&& message)
    : message_type_id(message_type_id), message(stdx::move(message)) {}
  Message(const Message& mess) = delete;
  Message(Message&& mess) : Message() { swap(*this, mess); }

  Message& operator=(Message mess) { swap(*this, mess); return *this; }
};

void swap(Message& a, Message& b) {
  stdx::swap(a.message_type_id, b.message_type_id);
  stdx::swap(a.message, b.message);
}



template <size_t BUFFER_SIZE=256>
class MessageConveyor
{
public:
  MessageConveyor(IMessageSocket* socket) : socket(socket) { }
  ~MessageConveyor() = default;

  bool connect() { return socket->connect(); }
  void disconnect() { return socket->disconnect(); }
   
  bool send(const msg::IMessage& message) {
    uint32_t message_length = sizeof(uint16_t) + message.getSize();
    uint8_t* data_buffer = serialize(message_length, send_buffer);
    data_buffer = serialize(message.getMessageType(), data_buffer);
    data_buffer = message.serialize(data_buffer);

    return socket->send(send_buffer, sizeof(message_length) + message_length);
  }

  Message processIncomingMessage() {
    // TODO(cenz): return ptr to actual message begin position in the buffer?
    // TODO(cenz): find a better name for this function
    bool is_complete_message = 
      stream_reader.processIncomingBytes(*socket, receive_buffer, BUFFER_SIZE);
    if (!is_complete_message)
      return { 0,nullptr };

    const uint8_t* message_begin = receive_buffer + sizeof(uint32_t);
    return bufferToMessage(message_begin);
    // TODO(cenz): dispatch message with the correct type
    //auto& callbacks = callback_registry[message.type];
    //for (auto& callback : callbacks) {
    //  callback(message);
    //}
  }

  /*
  void registerCallback(MessageType type, Callback callback) {
    callback_registry[type].push_back(callback);
  }
  */

private:
  uint8_t send_buffer[BUFFER_SIZE];
  uint8_t receive_buffer[BUFFER_SIZE];

  IMessageSocket* socket;  
  StreamMessageReader stream_reader;

  Message bufferToMessage(const uint8_t* buffer) {
    uint16_t type;
    buffer = deserialize(buffer, type);
    if (!buffer) {
      // TODO(cenz): deal with synchronization issues
      // com::MessageType deserialization failed! It is likely that
      // the reading stream is not properly aligned with incoming messages.
      // It can happen if the underlying socket has dropped some bytes.
    }

    auto& registry = msg::MessageRegistry::getInstance();

    const uint8_t* new_buffer_ptr;
    stdx::UPtr<msg::IMessage> message = registry.deserializeMessage(type, buffer, &new_buffer_ptr);
    return { type, stdx::move(message) };
  }
};


}