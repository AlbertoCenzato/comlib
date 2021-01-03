#pragma once

#include "communication_interface.h"
#include "stream_message_reader.h"
#include "utils.h"
#include "serialization.h"
#include <chrono>
#include <map>
#include <vector>
#include <optional>

namespace com {

namespace internal {

void fillMessage(Message& message, MessageType type, const uint8_t* data);

}


template <size_t BUFFER_SIZE=256>
class MessageConveyor
{
public:
  using Callback = void(*)(const Message&);

  MessageConveyor(IMessageSocket* socket) : socket(socket) { }
  ~MessageConveyor() = default;  // TODO(cenz): disconnect on destruction

  bool connect() { return socket->connect(); }
  void disconnect() { return socket->disconnect(); }

  bool send(const BinaryMessage& message) {
    uint32_t message_length = 
      sizeof(MessageType) + 
      sizeof(message.payload_length) + 
      message.payload_length;
    uint8_t* data_buffer = serialize(message_length, send_buffer);
    data_buffer = serialize(getMessageType<BinaryMessage>(), data_buffer);
    data_buffer = serialize(message.payload_length, data_buffer);
    memcpy(data_buffer, message.payload, message.payload_length);

    return socket->send(send_buffer, sizeof(message_length) + message_length);
  }
  
  bool send(const MoveMessage& message) {
    uint32_t message_length = sizeof(MessageType) + sizeof(message);

    uint8_t* data_buffer = serialize(message_length, send_buffer);
    data_buffer = serialize(getMessageType<MoveMessage>(), data_buffer);
    data_buffer = serialize(message.x, data_buffer);
    data_buffer = serialize(message.y, data_buffer);
    data_buffer = serialize(message.rot, data_buffer);

    return socket->send(send_buffer, sizeof(message_length) + message_length);
  }

  bool send(const EmptyMessage& message) {
    uint32_t message_length = sizeof(MessageType);
    uint8_t* data_buffer = serialize(message_length, send_buffer);
    data_buffer = serialize(getMessageType<EmptyMessage>(), data_buffer);

    return socket->send(send_buffer, sizeof(message_length) + message_length);
  }

  bool send(const Int32Message& message) {
    uint32_t message_length = sizeof(MessageType) + sizeof(message);
    uint8_t* data_buffer = serialize(message_length, send_buffer);
    data_buffer = serialize(getMessageType<Int32Message>(), data_buffer);
    data_buffer = serialize(message.value, data_buffer);

    return socket->send(send_buffer, sizeof(message_length) + message_length);
  }

  //void receive(Message& message) {
  //  MessageType type;
  //  size_t message_length = socket->receive(receive_buffer);
  //  const uint8_t* data_buffer = utils::deserialize(receive_buffer, type);
  //  internal::fillMessage(message, type, data_buffer);
  //}

  std::optional<Message> processIncomingMessage() {
    // TODO(cenz): return ptr to actual message begin position in the buffer?
    // TODO(cenz): find a better name for this function
    bool is_complete_message = 
      stream_reader.processIncomingBytes(*socket, receive_buffer, BUFFER_SIZE);
    if (!is_complete_message)
      return {};

    const uint8_t* message_begin = receive_buffer + sizeof(uint32_t);
    Message message;
    bufferToMessage(message_begin, message);
    return message;
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

  /*std::chrono::microseconds ping() {
    auto start = std::chrono::steady_clock::now();
    
    bool success = send(EmptyMessage{});
    if (!success)
      return std::chrono::microseconds{-1};
    Message reply;
    receive(reply);
    if (reply.type != MessageType::EMPTY_MESSAGE)
      return std::chrono::microseconds{ -1 };

    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  }*/

private:
  uint8_t send_buffer[BUFFER_SIZE];
  uint8_t receive_buffer[BUFFER_SIZE];

  IMessageSocket* socket;  
  StreamMessageReader stream_reader;
  //std::map<MessageType, std::vector<Callback>> callback_registry;

  void bufferToMessage(const uint8_t* buffer, Message& message) {
    MessageType type;
    buffer = deserialize(buffer, type);
    internal::fillMessage(message, type, buffer);
  }
};


}