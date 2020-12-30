#include "mock_message_socket.h"
#include "utils.h"

namespace com {

MockMessageSocket::MockMessageSocket(
  std::array<std::uint8_t, 1024>* send_buffer,
  std::array<std::uint8_t, 1024>* receive_buffer) 
  : is_connected(false), 
    send_buffer(send_buffer), 
    receive_buffer(receive_buffer), 
    alloc_send(send_buffer==nullptr), 
    alloc_receive(receive_buffer==nullptr) {
  if (alloc_send)
    send_buffer = new std::array<std::uint8_t, 1024>();
  if (alloc_receive)
    receive_buffer = new std::array<std::uint8_t, 1024>();
}

MockMessageSocket::~MockMessageSocket() {
  if (alloc_send)
    delete send_buffer;
  if (alloc_receive)
    delete receive_buffer;
}

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
  if (bytes > send_buffer->size())
    return false;

  utils::copyToLittleEndian(bytes, send_buffer->data());
  memcpy(send_buffer->data() + sizeof(bytes), data, bytes);
  return true;
}

std::size_t MockMessageSocket::receive(void* data) {
  std::size_t length = utils::copyFromLittleEndian<std::size_t>(receive_buffer->data());
  memcpy(data, receive_buffer->data() + sizeof(length), length);
  return length;
}

}