#pragma once

#include "communication_interface.h"
#include "utils.h"
#include <chrono>

namespace com {

namespace internal {

void fillMessage(Message& message, MessageType type, const std::uint8_t* data);

template <class T>
std::uint8_t* serialize(const T& value, std::uint8_t* buffer) {
  utils::copyToLittleEndian(value, buffer);
  return buffer + sizeof(T);
}

template<class T>
const std::uint8_t* deserialize(const std::uint8_t* buffer, T& value) {
  value = utils::copyFromLittleEndian<T>(buffer);
  return buffer + sizeof(T);
}

}


template <size_t BUFFER_SIZE=256>
class MessageConveyor
{
public:
  MessageConveyor(IMessageSocket* socket) : socket(socket) { }
  ~MessageConveyor() = default;

  bool connect() { return socket->connect(); }
  void disconnect() { return socket->disconnect(); }

  bool send(const BinaryMessage& message) {
    std::uint8_t* data_buffer = buffer;
    data_buffer = internal::serialize(message.type, buffer);
    data_buffer = internal::serialize(message.payload_length, data_buffer);
    memcpy(data_buffer, message.payload, message.payload_length);

    size_t message_length = sizeof(std::int16_t) + sizeof(message.payload_length) + message.payload_length;
    return socket->send(buffer, message_length);
  }
  
  bool send(const MoveMessage& message) {
    std::uint8_t* data_buffer = buffer;
    data_buffer = internal::serialize(message.type, buffer);
    data_buffer = internal::serialize(message.x, data_buffer);
    data_buffer = internal::serialize(message.y, data_buffer);
    data_buffer = internal::serialize(message.rot, data_buffer);

    size_t message_length = sizeof(std::int16_t) + sizeof(message);
    return socket->send(buffer, message_length);
  }

  void receive(Message& message) {
    size_t message_length = socket->receive(buffer);
    MessageType type;
    const std::uint8_t* data_buffer = internal::deserialize(buffer, type);
    internal::fillMessage(message, type, data_buffer);
  }

  std::chrono::microseconds ping() {
    return 0;
  }

private:
  std::uint8_t buffer[BUFFER_SIZE];
  IMessageSocket* socket;  
};


}