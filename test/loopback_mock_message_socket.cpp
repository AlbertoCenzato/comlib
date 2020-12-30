#include "loopback_mock_message_socket.h"

namespace com::test {

LoopbackMockMessageSocket::LoopbackMockMessageSocket()
  : send_receive_queue(),
  send_socket(&send_receive_queue, nullptr),
  receive_socket(nullptr, &send_receive_queue)
{

}

bool LoopbackMockMessageSocket::connect() {
  bool success = send_socket.connect();
  success &= receive_socket.connect();
  return success;
}

void LoopbackMockMessageSocket::disconnect() {
  send_socket.disconnect();
  receive_socket.disconnect();
}

bool LoopbackMockMessageSocket::send(const void* data, size_t bytes) {
  return send_socket.send(data, bytes);
}

size_t LoopbackMockMessageSocket::receive(void* data) {
  return receive_socket.receive(data);
}


}