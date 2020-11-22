#pragma once

#include "communication_interface.h"
#include <array>

namespace com {

class MockMessageSocket : public IMessageSocket 
{
public:
  MockMessageSocket();
  bool connect() override;
  void disconnect() override;
  bool send(const void* data, size_t bytes) override;
  size_t receive(void* data) override;

private:
  bool is_connected;
  std::array<std::uint8_t, 1024> send_receive_buffer;
};

}