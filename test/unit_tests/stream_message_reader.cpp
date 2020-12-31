#include "mocks/mock_message_socket.h"
#include <stream_message_reader.h>
#include <utils.h>

#include <gtest/gtest.h>

using namespace com;

TEST(StreamMessageReader, receiveOneMessage) {
  test::LoopbackMockMessageSocket socket;
  socket.connect();

  std::uint8_t receive_buffer[256];
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

  std::uint8_t receive_buffer[256];
  StreamMessageReader stream_reader;
  bool result = stream_reader.processIncomingBytes(socket, receive_buffer);
  EXPECT_FALSE(result);

  uint64_t message = 0x0123456789ABCDEF;
  size_t message_length = sizeof(message);
  socket.send(&message_length, sizeof(message_length));

  result = stream_reader.processIncomingBytes(socket, receive_buffer);
  EXPECT_FALSE(result);

  socket.send(&message, sizeof(message));
  socket.send(&message_length, sizeof(message_length));
  result = stream_reader.processIncomingBytes(socket, receive_buffer);
  EXPECT_TRUE(result);
  auto message_ptr = receive_buffer + sizeof(message_length);
  uint64_t received_message = com::utils::copyFromLittleEndian<uint64_t>(message_ptr);
  EXPECT_EQ(received_message, message);

  result = stream_reader.processIncomingBytes(socket, receive_buffer);
  EXPECT_FALSE(result);

  socket.send(&message, sizeof(message));
  result = stream_reader.processIncomingBytes(socket, receive_buffer);
  EXPECT_TRUE(result);
  message_ptr = receive_buffer + sizeof(message_length);
  received_message = com::utils::copyFromLittleEndian<uint64_t>(message_ptr);
  EXPECT_EQ(received_message, message);
}
