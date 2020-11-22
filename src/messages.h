#pragma once

#include <cstdint>
#include <cstddef>

enum class MessageType {
  ERROR = -1,
  ACK = 0,
  NACK = 1,
  PING = 2,
  LOG = 3,
  VELOCITY_VECTOR = 4,
  BINARY_MESSAGE = 5
};


struct BinaryMessage {
  std::size_t payload_length; 
  std::uint8_t* payload;

  void mallocAndSet(std::size_t payload_length, const std::uint8_t* payload) {
    this->payload_length = payload_length;
    freeMemory();
    this->payload = reinterpret_cast<std::uint8_t*>(malloc(payload_length));
    memcpy(this->payload, payload, payload_length);
  }

  void freeMemory() {
    if (payload)
      free(payload);
    payload = nullptr;
  }
};

struct Message {
  MessageType type;
  union {
    BinaryMessage binary;
  } message;

  static Message buildEmptyMessage() {
    Message m;
    m.type = MessageType::BINARY_MESSAGE;
    m.message.binary.payload_length = 0;
    m.message.binary.payload = nullptr;
    return m;
  }
};