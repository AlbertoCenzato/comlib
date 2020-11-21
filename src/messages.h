#pragma once

#include <cstdint>
#include <cstddef>

enum class MessageType {
  ERROR = -1,
  ACK = 0,
  NACK = 1,
  PING = 2,
  LOG = 3,
  VELOCITY_VECTOR = 4
};

struct BinaryMessage {
  MessageType type;
  std::size_t payload_length;
  std::uint8_t* payload;

  BinaryMessage(size_t payload_length) 
    : BinaryMessage(MessageType::ERROR, payload_length, nullptr) { }

  BinaryMessage(MessageType type, std::size_t payload_length, std::uint8_t* payload)
    : type(type), payload_length(payload_length) {
    this->payload = malloc(payload_length);
    if (payload)
      memcpy(this->payload, payload, payload_length);
  }

  ~BinaryMessage() {
    free(payload);
  }
};
