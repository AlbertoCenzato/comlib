#include "stream_message_reader.h"

namespace com {

bool StreamMessageReader::processIncomingBytes(IMessageSocket& socket, std::uint8_t* buffer) {

  size_t new_bytes = socket.receive(buffer + bytes_received);
  if (new_bytes <= 0)
    return false;

  bytes_received += static_cast<std::uint32_t>(new_bytes);  // TODO(cenz): either both size_t or both uint32_t
  const std::uint8_t* current_ptr = buffer + parsing_offset;

  while (true) {
    if (status == Status::SEEK) {
      if (bytes_received < sizeof(message_length)) {
        return false;
      }

      current_ptr = readLengthAndAdvance(current_ptr, message_length);
      // TODO(cenz): nonsense. change message_length from size_t to uint32_t in the buffer protocol
      // this way we are sure it has the same size on all platforms (arduino, raspberry, x64...)
      message_bytes_to_process = static_cast<std::uint32_t>(message_length);  
      status = Status::PARSING_MESSAGE;
      continue;
    }
    else if (status == Status::PARSING_MESSAGE) {
      if (message_bytes_to_process <= bytes_received) {  // all message received
        parsing_offset += message_bytes_to_process;
        bytes_received -= message_bytes_to_process;
        message_bytes_to_process = 0;
        message_length = 0;
        status = Status::SEEK;
        compactBuffer(buffer);
        return true;
      }
      else {  // we have to wait for another chunk of data

      }
      break;
    }
  }

  return true;
}

const std::uint8_t* StreamMessageReader::readLengthAndAdvance(const std::uint8_t* data, std::size_t& value) {
  auto new_current_ptr = utils::deserialize(data, value);
  auto processed_bytes = std::distance(data, new_current_ptr);
  parsing_offset += static_cast<std::uint32_t>(processed_bytes);  // TODO(cenz): cast?
  message_bytes_to_process -= static_cast<std::uint32_t>(processed_bytes);  // TODO(cenz): cast?
  return data + processed_bytes;
}

void StreamMessageReader::compactBuffer(std::uint8_t* buffer) {
  if (status == Status::SEEK && message_bytes_to_process > 0) {  // compact buffer to free space for new data
    utils::shiftBytesLeft(buffer + parsing_offset, bytes_received, parsing_offset);
    parsing_offset = 0;
  }
}


}