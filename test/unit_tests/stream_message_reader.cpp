#include "mocks/mock_message_socket.h"
#include <com/stream_message_reader.h>
#include <com/utils.h>

#include <gtest/gtest.h>

using namespace com;

TEST(StreamMessageReader, receiveOneMessage) {
  test::LoopbackMockMessageSocket socket;
  socket.connect();

  uint8_t receive_buffer[256];
  StreamMessageReader stream_reader;
  bool result = stream_reader.processIncomingBytes(socket, receive_buffer);
  EXPECT_FALSE(result);

  uint64_t message = 0x0123456789ABCDEF;
  size_t message_length = sizeof(message);
  socket.send(&message_length, sizeof(message_length));

  result = stream_reader.processIncomingBytes(socket, receive_buffer);
  EXPECT_FALSE(result);

  socket.send(&message, sizeof(message));
  result = stream_reader.processIncomingBytes(socket, receive_buffer);
  EXPECT_TRUE(result);
  auto message_ptr = receive_buffer + sizeof(message_length);
  uint64_t received_message = com::utils::copyFromLittleEndian<uint64_t>(message_ptr);
  EXPECT_EQ(received_message, message);
}

TEST(StreamMessageReader, receiveMultipleMessages) {
  test::LoopbackMockMessageSocket socket;
  socket.connect();

  // empty socket buffer
  uint8_t receive_buffer[256];
  StreamMessageReader stream_reader;
  bool result = stream_reader.processIncomingBytes(socket, receive_buffer);
  EXPECT_FALSE(result);

  // socket buffer contains first message length only
  uint64_t first_message = 0x0123456789ABCDEF;
  uint32_t first_message_length = sizeof(first_message);
  socket.send(&first_message_length, sizeof(first_message_length));
  result = stream_reader.processIncomingBytes(socket, receive_buffer);
  EXPECT_FALSE(result);

  // socket buffer contains first message length + payload + second message length
  socket.send(&first_message, sizeof(first_message));
  uint32_t second_message = 0x456789AB;
  uint32_t second_message_length = sizeof(second_message);
  socket.send(&second_message_length, sizeof(second_message_length));
  result = stream_reader.processIncomingBytes(socket, receive_buffer);
  EXPECT_TRUE(result);
  auto message_ptr = receive_buffer + sizeof(first_message_length);
  uint32_t received_first_message_length = utils::copyFromLittleEndian<uint32_t>(receive_buffer);
  uint64_t received_first_message = com::utils::copyFromLittleEndian<uint64_t>(message_ptr);
  EXPECT_EQ(received_first_message_length, first_message_length);
  EXPECT_EQ(received_first_message, first_message);
  
  // socket buffer has no new data but buffer gets compacted
  result = stream_reader.processIncomingBytes(socket, receive_buffer);
  EXPECT_FALSE(result);
  uint32_t received_second_message_length = utils::copyFromLittleEndian<uint32_t>(receive_buffer);
  EXPECT_EQ(received_second_message_length, second_message_length);

  // socket buffer contains second message
  socket.send(&second_message, sizeof(second_message));
  result = stream_reader.processIncomingBytes(socket, receive_buffer);
  EXPECT_TRUE(result);
  auto second_message_ptr = receive_buffer + sizeof(second_message_length);
  received_second_message_length = utils::copyFromLittleEndian<uint32_t>(receive_buffer);
  uint32_t received_second_message = utils::copyFromLittleEndian<uint32_t>(second_message_ptr);
  EXPECT_EQ(received_second_message_length, second_message_length);
  EXPECT_EQ(received_second_message, second_message);
}
