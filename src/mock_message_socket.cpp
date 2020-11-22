#include "mock_message_socket.h"
#include "utils.h"

namespace com {

MockMessageSocket::MockMessageSocket() : is_connected(false){}

bool MockMessageSocket::connect() {
  if (is_connected)
    return false;
  is_connected = true;
  return true;
}

void MockMessageSocket::disconnect() {
  is_connected = false;
}

bool MockMessageSocket::send(const void* data, std::size_t bytes) {
  if (bytes > send_receive_buffer.size())
    return false;

  utils::copyToBytesLittleEndian(bytes, send_receive_buffer.data());
  memcpy(send_receive_buffer.data() + sizeof(bytes), data, bytes);
  return true;
}

std::size_t MockMessageSocket::receive(void* data) {
  std::size_t length = utils::copyFromLittleEndianBytes<std::size_t>(send_receive_buffer.data());
  memcpy(data, send_receive_buffer.data() + sizeof(length), length);
  return length;
}

}