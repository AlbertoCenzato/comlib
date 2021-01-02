#include "mock_message_socket.h"

#include <com/utils.h>

namespace com::test {

// ------------ MockMessageSocket ----------------

MockMessageSocket::MockMessageSocket(
  std::queue<uint8_t>* send_queue,
  std::queue<uint8_t>* receive_queue) 
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

bool MockMessageSocket::send(const void* data, uint32_t bytes) {
  if (send_queue->size() + bytes > std::numeric_limits<uint32_t>::max())
    return false;  // queue has reached its maximum capacity
  auto data_ptr = reinterpret_cast<const uint8_t*>(data);
  for (uint32_t i = 0; i < bytes; ++i) {
    send_queue->push(data_ptr[i]);
  }
  return true;
}

uint32_t MockMessageSocket::receive(void* data) {
  size_t data_length = receive_queue->size();
  auto data_ptr = reinterpret_cast<uint8_t*>(data);
  for (size_t i = 0; i < data_length; ++i) {
    data_ptr[i] = receive_queue->front();
    receive_queue->pop();
  }
  return static_cast<uint32_t>(data_length);
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

bool LoopbackMockMessageSocket::send(const void* data, uint32_t bytes) {
  return send_socket.send(data, bytes);
}

uint32_t LoopbackMockMessageSocket::receive(void* data) {
  return receive_socket.receive(data);
}

}