#pragma once

#include "mock_message_socket.h"

namespace com::test {

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