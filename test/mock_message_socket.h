#pragma once

#include "communication_interface.h"
#include <queue>

namespace com::test {

class MockMessageSocket : public IMessageSocket 
{
public:
  MockMessageSocket(
    std::queue<std::uint8_t>* send_queue, 
    std::queue<std::uint8_t>* receive_queue);
  ~MockMessageSocket() = default;
  bool connect() override;
  void disconnect() override;
  bool send(const void* data, size_t bytes) override;
  size_t receive(void* data) override;

private:
  bool is_connected;
  std::queue<std::uint8_t>* send_queue;
  std::queue<std::uint8_t>* receive_queue;
};


class LoopbackMockMessageSocket : public IMessageSocket
{
public:
  LoopbackMockMessageSocket();
  bool connect() override;
  void disconnect() override;
  bool send(const void* data, size_t bytes) override;
  size_t receive(void* data) override;

private:
  std::queue<std::uint8_t> send_receive_queue;
  MockMessageSocket send_socket;
  MockMessageSocket receive_socket;
};

}