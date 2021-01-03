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

uint32_t MockMessageSocket::receive(void* data, uint32_t bytes) {
  auto data_length = static_cast<uint32_t>(receive_queue->size());
  auto data_ptr = reinterpret_cast<uint8_t*>(data);
  const uint32_t bytes_to_read = std::min(data_length, bytes);
  for (uint32_t i = 0; i < bytes_to_read; ++i) {
    data_ptr[i] = receive_queue->front();
    receive_queue->pop();
  }
  return bytes_to_read;
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

uint32_t LoopbackMockMessageSocket::receive(void* data, uint32_t bytes) {
  return receive_socket.receive(data, bytes);
}


bool ThreadsafeLoopbackSocket::connect() {
  std::lock_guard<std::mutex> lock{ mutex };
  return LoopbackMockMessageSocket::connect();
}

void ThreadsafeLoopbackSocket::disconnect() {
  std::lock_guard<std::mutex> lock{ mutex };
  LoopbackMockMessageSocket::disconnect();
  return;
}

bool ThreadsafeLoopbackSocket::send(const void* data, uint32_t bytes) {
  std::lock_guard<std::mutex> lock{ mutex };
  return LoopbackMockMessageSocket::send(data, bytes);
}

uint32_t ThreadsafeLoopbackSocket::receive(void* data, uint32_t bytes) {
  std::lock_guard<std::mutex> lock{ mutex };
  return LoopbackMockMessageSocket::receive(data, bytes);
}

}