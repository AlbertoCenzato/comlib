#include "mocks/mock_message_socket.h"

#include <communication.h>
#include <gtest/gtest.h>

using namespace com;

TEST(MessageConveyor, sendReceiveBinary) {
  test::LoopbackMockMessageSocket socket;
  MessageConveyor<256> converyor{ &socket };

  std::uint64_t payload = 0x0123456789ABCDEF;
  BinaryMessage message{ sizeof(payload), utils::toUBytesArray(payload) };
  converyor.send(message);

  Message reply = Message::buildEmptyMessage();
  converyor.receive(reply);

  EXPECT_EQ(reply.type, MessageType::BINARY_MESSAGE);
  EXPECT_EQ(reply.message.binary.payload_length, message.payload_length);
  EXPECT_EQ(utils::fromUBytesArray<std::uint64_t>(reply.message.binary.payload), payload);

  if (reply.type == MessageType::BINARY_MESSAGE)
    reply.message.binary.freeMemory();
}

TEST(MessageConveyor, sendReceiveMove) {
  test::LoopbackMockMessageSocket socket;
  MessageConveyor<265> conveyor{ &socket };

  MoveMessage message{ 1.f, 0.f, 0.5f };
  conveyor.send(message);

  Message reply = Message::buildEmptyMessage();
  conveyor.receive(reply);

  EXPECT_EQ(reply.type, MessageType::MOVE_MESSAGE);
  EXPECT_EQ(reply.message.move.x, 1.f);
  EXPECT_EQ(reply.message.move.y, 0.f);
  EXPECT_EQ(reply.message.move.rot, 0.5f);
}


TEST(MessageConveyor, sendReceivePing) {
  test::LoopbackMockMessageSocket socket;
  MessageConveyor<265> conveyor{ &socket };

  std::chrono::microseconds us_elapsed = conveyor.ping();
  std::cout << "RTT: " << us_elapsed.count() / 1000.0 << "ms" << std::endl;
  EXPECT_NE(us_elapsed.count(), -1);
}