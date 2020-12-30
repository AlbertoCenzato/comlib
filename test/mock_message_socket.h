#pragma once

#include "communication_interface.h"
#include <array>

namespace com::test {

class MockMessageSocket : public IMessageSocket 
{
public:
  MockMessageSocket(
    std::array<std::uint8_t, 1024>* send_buffer = nullptr, 
    std::array<std::uint8_t, 1024>* receive_buffer = nullptr);
  ~MockMessageSocket();
  bool connect() override;
  void disconnect() override;
  bool send(const void* data, size_t bytes) override;
  size_t receive(void* data) override;

private:
  const bool alloc_send;
  const bool alloc_receive;
  bool is_connected;
  std::array<std::uint8_t, 1024>* send_buffer;
  std::array<std::uint8_t, 1024>* receive_buffer;
};

}