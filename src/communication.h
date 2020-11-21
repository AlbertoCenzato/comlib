#include "communication_interface.h"
#include "utils.h"


namespace com {

template <size_t BUFFER_SIZE=256>
class MessageConveyor
{
public:
  MessageConveyor(IMessageSocket* socket) : socket(socket), buffer({0}) { }
  ~MessageConveyor() = default;

  bool connect() { return socket->connect(); }
  void disconnect() { return socket->disconnect(); }

  bool send(const BinaryMessage& message) {
    size_t message_length = sizeof(message.type) + sizeof(message.payload_length) + message.payload_length;
    
    std::uint8_t* data_buffer = buffer;
    utils::copyToBytesLittleEndian(static_cast<std::int32_t>(message.type), data_buffer);
    data_buffer += sizeof(std::int32_t);
    utils::copyToBytesLittleEndian(message.payload_length, data_buffer);
    data_buffer += sizeof(message.payload_length);
    memcpy(data_buffer, message.payload);

    return socket->send(buffer, message_length);
  }

  BinaryMessage receive() {
    size_t message_length = socket->receive(buffer);
    MessageType type = static_cast<MessageType>(*reinterpret_cast<std::int32_t*>(buffer));
    size_t payload_length = *reinterpret_cast<std::size_t*>(buffer + sizeof(std::int32_t));
    std::uint8_t* payload = reinterpret_cast<std::uint8_t*>(buffer + sizeof(std::int32_t) + sizeof(std::size_t));
    return { type, payload_length, payload };
  }

  std::chrono::microseconds ping() {
    return 0;
  }

private:
  std::uint8_t buffer[BUFFER_SIZE];
  IMessageSocket* socket;
};


}