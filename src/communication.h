#include "communication_interface.h"

namespace com {

template <size_t BUFFER_SIZE=256>
class MessageConveyor
{
public:
  MessageConveyor(IMessageSocket* socket) : socket(socket), buffer({0}) { }
  ~MessageConveyor() = default;

  bool connect() { return socket->connect(); }
  void disconnect() { return socket->disconnect(); }

  bool send(const Message& message) {
    size_t message_length = sizeof(message.type) + message.length;
    return false;
  }

  Message receive() {
    return Message{0};
  }

  std::chrono::microseconds ping() {
    return 0;
  }

private:
  std::uint8_t buffer[BUFFER_SIZE];
  IMessageSocket* socket;
};


}