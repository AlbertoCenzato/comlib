#pragma once

#include "communication_interface.h"
#include "utils.h"
#include <chrono>

namespace com {

namespace internal {

void fillMessage(Message& message, MessageType type, const std::uint8_t* data);

}


template <size_t BUFFER_SIZE=256>
class MessageConveyor
{
public:
  using Callback = void(*)(const Message&);
  MessageConveyor(IMessageSocket* socket) : socket(socket) { }
  ~MessageConveyor() = default;

  bool connect() { return socket->connect(); }
  void disconnect() { return socket->disconnect(); }

  bool send(const BinaryMessage& message) {
    std::uint8_t* data_buffer = send_buffer;
    data_buffer = utils::serialize(message.type, send_buffer);
    data_buffer = utils::serialize(message.payload_length, data_buffer);
    memcpy(data_buffer, message.payload, message.payload_length);

    size_t message_length = sizeof(std::int16_t) + sizeof(message.payload_length) + message.payload_length;
    return socket->send(send_buffer, message_length);
  }
  
  bool send(const MoveMessage& message) {
    std::uint8_t* data_buffer = send_buffer;
    data_buffer = utils::serialize(message.type, send_buffer);
    data_buffer = utils::serialize(message.x, data_buffer);
    data_buffer = utils::serialize(message.y, data_buffer);
    data_buffer = utils::serialize(message.rot, data_buffer);

    size_t message_length = sizeof(std::int16_t) + sizeof(message);
    return socket->send(send_buffer, message_length);
  }

  bool send(const EmptyMessage& message) {
    std::uint8_t* data_buffer = send_buffer;
    data_buffer = utils::serialize(message.type, send_buffer);

    size_t message_length = sizeof(std::int16_t);
    return socket->send(send_buffer, message_length);
  }

  void receive(Message& message) {
    size_t message_length = socket->receive(receive_buffer);
    MessageType type;
    const std::uint8_t* data_buffer = utils::deserialize(receive_buffer, type);
    internal::fillMessage(message, type, data_buffer);
  }

  std::chrono::microseconds ping() {
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
  }

private:
  std::uint8_t send_buffer[BUFFER_SIZE];
  std::uint8_t receive_buffer[BUFFER_SIZE];
  IMessageSocket* socket;  
};


}