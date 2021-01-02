#include "stream_message_reader.h"
#include "serialization.h"

namespace com {

StreamMessageReader::StreamMessageReader()
  : status(Status::SEEK), 
    parsing_offset(0), 
    bytes_received(0), 
    message_length(0), 
    buffer_should_be_compacted(false)
{

}

bool StreamMessageReader::processIncomingBytes(IMessageSocket& socket, uint8_t* buffer) {
  if (buffer_should_be_compacted) {
    compactBuffer(buffer);
    buffer_should_be_compacted = false;
  }

  uint32_t new_bytes = socket.receive(buffer + bytes_received);
  if (new_bytes <= 0)
    return false;

  bytes_received += new_bytes;
  return processData(buffer);
}

bool StreamMessageReader::processData(uint8_t* buffer) {
  if (status == Status::SEEK) {
    bool seek_ok = seek(buffer);
    if (seek_ok) {  
      status = Status::PARSING_MESSAGE;  // if seek succeeds, set current status to PARSING_MESSAGE
      return processData(buffer);        // and continue processing data
    }
    return false;
  }
  else if (status == Status::PARSING_MESSAGE) {
    bool parsing_ok = parseMessage(buffer);
    if (parsing_ok) {
      status = Status::SEEK;              // if message parsing succeeds set current status to SEEK
      buffer_should_be_compacted = true;  // and set flag to compact buffer next time processIncomingBytes is called
    }
    return parsing_ok;
  }

  return false;  // NOTE: this statement should be unreachable
}

bool StreamMessageReader::seek(const uint8_t* buffer) {
  if (bytes_received < sizeof(message_length))
    return false;

  auto advanced_buffer_ptr = deserialize(buffer, message_length);
  parsing_offset += static_cast<uint32_t>(advanced_buffer_ptr - buffer);  // TODO(cenz): cast?

  return true;
}

bool StreamMessageReader::parseMessage(const uint8_t* buffer) {
  // NOTE: -parsing_offset to not take into account message length data
  if (message_length > bytes_received - parsing_offset)  
    return false;  // we have to wait for another chunk of data
  
  bytes_received -= (parsing_offset + message_length);  // remove message length + payload from bytes received
  parsing_offset += message_length;  // advance parsing_offset to skip already validated message
  message_length = 0;
  return true;
}

void StreamMessageReader::compactBuffer(uint8_t* buffer) {
  utils::shiftBytesLeft(buffer + parsing_offset, bytes_received, parsing_offset);
  parsing_offset = 0;
}


}