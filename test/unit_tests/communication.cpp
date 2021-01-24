#include "mocks/mock_message_socket.h"

#include <com/communication.h>
#include <com/msg/int32_message.h>
#include <com/msg/move_message.h>

#include <gtest/gtest.h>

using namespace com;

TEST(MessageConveyor, sendInt32Message) {
  test::LoopbackMockMessageSocket socket;
  MessageConveyor<256> conveyor{ &socket };

  const int32_t payload = 0x01234567;
  msg::Int32Message message_sent{ payload };
  bool result = conveyor.send(message_sent);
  EXPECT_TRUE(result);

  const uint32_t BUFFER_SIZE = 256;
  uint8_t receive_buffer[BUFFER_SIZE];
  uint32_t bytes_received = socket.receive(receive_buffer, BUFFER_SIZE);

  //                                            = message length bytes + message type bytes + message bytes
  const uint32_t expected_message_size_in_bytes = sizeof(uint32_t) + sizeof(int16_t) + message_sent.getSize();
  EXPECT_EQ(bytes_received, expected_message_size_in_bytes);

  uint32_t message_length;
  msg::MessageType message_type;
  int32_t received_payload;
  auto buff_ptr = deserialize<uint32_t>(receive_buffer, message_length);
  buff_ptr = deserialize<msg::MessageType>(buff_ptr, message_type);
  deserialize<int32_t>(buff_ptr, received_payload);
  EXPECT_EQ(message_sent.getSize() + sizeof(msg::MessageType), message_length);
  EXPECT_EQ(message_sent.getMessageType(), message_type);
  EXPECT_EQ(payload, received_payload);
}

int32_t received_value = 0;

void callbackInt32(const msg::IMessage& message) {
  const auto& int32_message = dynamic_cast<const msg::Int32Message&>(message);
  received_value = int32_message.value;
}

TEST(MessageConveyor, sendReceiveInt32Message) {
  test::LoopbackMockMessageSocket socket;
  MessageConveyor<256> conveyor{ &socket };

  conveyor.registerCallback<msg::Int32Message>(callbackInt32);

  const int32_t payload = 0x01234567;
  msg::Int32Message message_sent{ payload };
  conveyor.send(message_sent);

  conveyor.processIncomingMessage();
  EXPECT_EQ(received_value, payload);
}


float x, y, rot;

void callbackMove(const msg::IMessage& message) {
  const auto& move_message = dynamic_cast<const msg::MoveMessage&>(message);
  x = move_message.x;
  y = move_message.y;
  rot = move_message.rot;
}

TEST(MessageConveyor, sendReceiveMoveMessage) {
  test::LoopbackMockMessageSocket socket;
  MessageConveyor<265> conveyor{ &socket };
  
  conveyor.registerCallback<msg::MoveMessage>(callbackMove);

  msg::MoveMessage message_sent{ 1.f, 0.f, 0.5f };
  conveyor.send(message_sent);

  conveyor.processIncomingMessage();

  EXPECT_EQ(x, 1.f);
  EXPECT_EQ(y, 0.f);
  EXPECT_EQ(rot, 0.5f);
}
