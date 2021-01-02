#include "mocks/mock_message_socket.h"

#include <com/utils.h>

#include <gtest/gtest.h>

using namespace com::test;

TEST(MockMessageSocket, connect) {
  MockMessageSocket socket{ nullptr, nullptr };
  EXPECT_TRUE(socket.connect());
}

TEST(MockMessageSocket, send) {
  std::queue<uint8_t> send_q, receive_q;
  MockMessageSocket socket{ &send_q, &receive_q };
  socket.connect();

  uint64_t value = 0x0123456789ABCDEF;
  socket.send(&value, sizeof(value));

  EXPECT_EQ(send_q.size(), sizeof(value));
  uint8_t tmp[sizeof(value)];
  for (size_t i = 0; i < sizeof(value); ++i) {
    tmp[i] = send_q.front();
    send_q.pop();
  }
  uint64_t retrieved_value = com::utils::fromLittleEndian<uint64_t>(tmp);

  EXPECT_EQ(value, retrieved_value);
}

TEST(MockMessageSocket, receive) {
  std::queue<uint8_t> send_q, receive_q;
  MockMessageSocket socket{ &send_q, &receive_q };
  socket.connect();

  uint64_t value = 0x0123456789ABCDEF;
  uint8_t tmp[sizeof(value)];
  com::utils::toLittleEndian(value, tmp);
  for (size_t i = 0; i < sizeof(value); ++i) {
    receive_q.push(tmp[i]);
  }

  uint8_t tmp_2[sizeof(value)];
  size_t bytes_received = socket.receive(tmp_2);
  EXPECT_EQ(sizeof(value), bytes_received);

  uint64_t retrieved_value = com::utils::fromLittleEndian<uint64_t>(tmp_2);
  EXPECT_EQ(value, retrieved_value);
}

TEST(LoopbackMockMessageSocket, connect) {
  LoopbackMockMessageSocket socket{};
  EXPECT_TRUE(socket.connect());
}

TEST(LoopbackMockMessageSocket, sendReceive) {
  LoopbackMockMessageSocket socket{};

  uint64_t value = 0x0123456789ABCDEF;
  socket.send(&value, sizeof(value));

  uint8_t tmp[sizeof(value)];
  size_t bytes_received = socket.receive(tmp);
  EXPECT_EQ(bytes_received, sizeof(value));
  uint64_t received_value = com::utils::fromLittleEndian<uint64_t>(tmp);
  EXPECT_EQ(value, received_value);

}
