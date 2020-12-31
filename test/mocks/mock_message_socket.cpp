#include "mock_message_socket.h"
#include "utils.h"

namespace com::test {

// ------------ MockMessageSocket ----------------

MockMessageSocket::MockMessageSocket(
  std::queue<std::uint8_t>* send_queue,
  std::queue<std::uint8_t>* receive_queue) 
  : is_connected(false), 
    send_queue(send_queue), 
    receive_queue(receive_queue)
{
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
  auto data_ptr = reinterpret_cast<const std::uint8_t*>(data);
  for (std::size_t i = 0; i < bytes; ++i) {
    send_queue->push(data_ptr[i]);
  }
  return true;
}

std::size_t MockMessageSocket::receive(void* data) {
  size_t data_length = receive_queue->size();
  auto data_ptr = reinterpret_cast<std::uint8_t*>(data);
  for (size_t i = 0; i < data_length; ++i) {
    data_ptr[i] = receive_queue->front();
    receive_queue->pop();
  }
  return data_length;
}

// ----------------- LoopbackMockMessageSocket -------------------------

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