#include <gtest/gtest.h>

#include <communication.h>
#include <mock_message_socket.h>

using namespace com;

TEST(MessageConveyor, sendReceive) {
  MockMessageSocket socket;
  MessageConveyor<256> converyor{ &socket };

  std::uint64_t payload = 0x0123456789ABCDEF;
  BinaryMessage message{ MessageType::LOG, sizeof(payload), utils::toUBytesArray(payload) };
  converyor.send(message);

  BinaryMessage reply = converyor.receive();

  EXPECT_EQ(reply.type, message.type);
  EXPECT_EQ(reply.payload_length, message.payload_length);
  EXPECT_EQ(utils::fromUBytesArray<std::uint64_t>(reply.payload), payload);
}