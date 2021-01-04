#include "mocks/mock_message_socket.h"

#include <com/communication.h>

#include <gtest/gtest.h>

using namespace com;

TEST(MessageConveyor, sendInt32Message) {
  test::LoopbackMockMessageSocket socket;
  MessageConveyor<256> conveyor{ &socket };

  const int32_t payload = 0x01234567;
  Int32Message message_sent{ payload };
  bool result = conveyor.send(message_sent);
  EXPECT_TRUE(result);

  const uint32_t BUFFER_SIZE = 256;
  uint8_t receive_buffer[BUFFER_SIZE];
  uint32_t bytes_received = socket.receive(receive_buffer, BUFFER_SIZE);

  //                                            = message length bytes + message type bytes + message bytes
  const uint32_t expected_message_size_in_bytes = sizeof(uint32_t) + sizeof(int16_t) + sizeof(Int32Message);
  EXPECT_EQ(bytes_received, expected_message_size_in_bytes);

  uint32_t message_length;
  MessageType message_type;
  int32_t received_payload;
  auto buff_ptr = deserialize<uint32_t>(receive_buffer, message_length);
  buff_ptr = deserialize<MessageType>(buff_ptr, message_type);
  deserialize<int32_t>(buff_ptr, received_payload);
  EXPECT_EQ(sizeof(Int32Message) + sizeof(MessageType), message_length);
  EXPECT_EQ(getMessageType<Int32Message>(), message_type);
  EXPECT_EQ(payload, received_payload);
}

TEST(MessageConveyor, sendReceiveInt32Message) {
  test::LoopbackMockMessageSocket socket;
  MessageConveyor<256> conveyor{ &socket };

  const int32_t payload = 0x01234567;
  Int32Message message_sent{ payload };
  conveyor.send(message_sent);

  Message message;
  bool has_message = conveyor.processIncomingMessage(message);

  EXPECT_TRUE(has_message);
  EXPECT_EQ(message.type, MessageType::INT32_MESSAGE);
  
  Int32Message& message_received = message.message.int32;
  EXPECT_EQ(message_received.value, payload);
}

TEST(MessageConveyor, sendReceiveMoveMessage) {
  test::LoopbackMockMessageSocket socket;
  MessageConveyor<265> conveyor{ &socket };

  MoveMessage message_sent{ 1.f, 0.f, 0.5f };
  conveyor.send(message_sent);

  Message message;
  bool has_message = conveyor.processIncomingMessage(message);
  EXPECT_TRUE(has_message);
  EXPECT_EQ(message.type, MessageType::MOVE_MESSAGE);

  MoveMessage& message_received = message.message.move;
  EXPECT_EQ(message_received.x, 1.f);
  EXPECT_EQ(message_received.y, 0.f);
  EXPECT_EQ(message_received.rot, 0.5f);
}
