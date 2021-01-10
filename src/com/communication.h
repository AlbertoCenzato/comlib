#pragma once

#include "communication_interface.h"
#include "msg/messages.h"
#include "stream_message_reader.h"
#include "utils.h"
#include "serialization.h"
#include <string.h>

namespace com {

namespace internal {

void fillMessage(msg::Message& message, msg::MessageType type, const uint8_t* data);

}


template <size_t BUFFER_SIZE=256>
class MessageConveyor
{
public:
  using Callback = void(*)(const msg::Message&);

  MessageConveyor(IMessageSocket* socket) : socket(socket) { }
  ~MessageConveyor() = default;  // TODO(cenz): disconnect on destruction

  bool connect() { return socket->connect(); }
  void disconnect() { return socket->disconnect(); }
    
  bool send(const msg::MoveMessage& message) {
    uint32_t message_length = sizeof(msg::MessageType) + message.size();
    uint8_t* data_buffer = serialize(message_length, send_buffer);
    data_buffer = serialize(msg::getMessageType<msg::MoveMessage>(), data_buffer);
    data_buffer = serialize(message.x, data_buffer);
    data_buffer = serialize(message.y, data_buffer);
    data_buffer = serialize(message.rot, data_buffer);

    return socket->send(send_buffer, sizeof(message_length) + message_length);
  }

  bool send(const msg::EmptyMessage& message) {
    uint32_t message_length = sizeof(msg::MessageType) + message.size();
    uint8_t* data_buffer = serialize(message_length, send_buffer);
    data_buffer = serialize(msg::getMessageType<msg::EmptyMessage>(), data_buffer);

    return socket->send(send_buffer, sizeof(message_length) + message_length);
  }

  bool send(const msg::Int32Message& message) {
    uint32_t message_length = sizeof(msg::MessageType) + message.size();
    uint8_t* data_buffer = serialize(message_length, send_buffer);
    data_buffer = serialize(msg::getMessageType<msg::Int32Message>(), data_buffer);
    data_buffer = serialize(message.value, data_buffer);

    return socket->send(send_buffer, sizeof(message_length) + message_length);
  }

  //void receive(Message& message) {
  //  MessageType type;
  //  size_t message_length = socket->receive(receive_buffer);
  //  const uint8_t* data_buffer = utils::deserialize(receive_buffer, type);
  //  internal::fillMessage(message, type, data_buffer);
  //}

  bool processIncomingMessage(msg::Message& message) {
    // TODO(cenz): return ptr to actual message begin position in the buffer?
    // TODO(cenz): find a better name for this function
    bool is_complete_message = 
      stream_reader.processIncomingBytes(*socket, receive_buffer, BUFFER_SIZE);
    if (!is_complete_message)
      return false;

    const uint8_t* message_begin = receive_buffer + sizeof(uint32_t);
    bufferToMessage(message_begin, message);
    return true;
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

  void bufferToMessage(const uint8_t* buffer, msg::Message& message) {
    msg::MessageType type;
    buffer = deserialize(buffer, type);
    if (!buffer) {
      // TODO(cenz): deal with synchronization issues
      // com::MessageType deserialization failed! It is likely that
      // the reading stream is not properly aligned with incoming messages.
      // It can happen if the underlying socket has dropped some bytes.
    }
    internal::fillMessage(message, type, buffer);
  }
};


}