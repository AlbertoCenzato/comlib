#include <gtest/gtest.h>

#include <communication.h>
#include <mock_message_socket.h>

using namespace com;

TEST(MessageConveyor, sendReceive) {
  MockMessageSocket socket;
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