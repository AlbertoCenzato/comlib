#include "stream_message_reader.h"

namespace com {

StreamMessageReader::StreamMessageReader()
  : status(Status::SEEK), 
    parsing_offset(0), 
    bytes_received(0), 
    message_length(0), 
    message_bytes_to_process(0)
{

}

bool StreamMessageReader::processIncomingBytes(IMessageSocket& socket, std::uint8_t* buffer) {
  size_t new_bytes = socket.receive(buffer + bytes_received);
  if (new_bytes <= 0)
    return false;

  bytes_received += static_cast<std::uint32_t>(new_bytes);  // TODO(cenz): either both size_t or both uint32_t
  return processData(buffer);
}

bool StreamMessageReader::processData(std::uint8_t* buffer) {
  if (status == Status::SEEK) {
    bool seek_ok = seek(buffer);
    if (!seek_ok)
      return false;
    return processData(buffer);
  }
  else if (status == Status::PARSING_MESSAGE) {
    bool parsing_ok = parseMessage(buffer);
    if (parsing_ok)
      compactBuffer(buffer);
    return parsing_ok;
  }

  return false;  // NOTE: this statement should be unreachable
}

bool StreamMessageReader::seek(const std::uint8_t* buffer) {
  if (bytes_received < sizeof(message_length))
    return false;

  auto advanced_buffer_ptr = utils::deserialize(buffer, message_length);
  parsing_offset += static_cast<std::uint32_t>(advanced_buffer_ptr - buffer);  // TODO(cenz): cast?

  // TODO(cenz): nonsense. change message_length from size_t to uint32_t in the buffer protocol
  // this way we are sure it has the same size on all platforms (arduino, raspberry, x64...)
  message_bytes_to_process = static_cast<std::uint32_t>(message_length);
  status = Status::PARSING_MESSAGE;
  return true;
}

bool StreamMessageReader::parseMessage(const std::uint8_t* buffer) {
  // NOTE: -parsing_offset to not take into account message length data
  if (message_bytes_to_process > bytes_received - parsing_offset)  
    return false;  // we have to wait for another chunk of data
  
  parsing_offset += message_bytes_to_process;
  bytes_received -= message_bytes_to_process;
  message_bytes_to_process = 0;
  message_length = 0;
  status = Status::SEEK;
  return true;
}

void StreamMessageReader::compactBuffer(std::uint8_t* buffer) {
  // compact buffer to free space for new data
  utils::shiftBytesLeft(buffer + parsing_offset, bytes_received, parsing_offset);
  parsing_offset = 0;
}


}