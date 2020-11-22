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
  MessageConveyor(IMessageSocket* socket) : socket(socket) { }
  ~MessageConveyor() = default;

  bool connect() { return socket->connect(); }
  void disconnect() { return socket->disconnect(); }

  bool send(const BinaryMessage& message) {
    size_t message_length = sizeof(std::int16_t) + sizeof(message.payload_length) + message.payload_length;
    
    std::uint8_t* data_buffer = buffer;
    utils::copyToBytesLittleEndian(static_cast<std::int16_t>(MessageType::BINARY_MESSAGE), data_buffer);
    data_buffer += sizeof(std::int16_t);
    utils::copyToBytesLittleEndian(message.payload_length, data_buffer);
    data_buffer += sizeof(message.payload_length);
    memcpy(data_buffer, message.payload, message.payload_length);

    return socket->send(buffer, message_length);
  }

  void receive(Message& message) {
    size_t message_length = socket->receive(buffer);
    MessageType type = static_cast<MessageType>(utils::fromUBytesArray<std::int16_t>(buffer));
    internal::fillMessage(message, type, buffer + sizeof(std::int16_t));
  }

  std::chrono::microseconds ping() {
    return 0;
  }

private:
  std::uint8_t buffer[BUFFER_SIZE];
  IMessageSocket* socket;  
};


}