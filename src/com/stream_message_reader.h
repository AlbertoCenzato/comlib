#pragma once

#include "communication_interface.h"
#include "utils.h"

namespace com {

class StreamMessageReader {
public:
  StreamMessageReader();
  ~StreamMessageReader() = default;

  bool processIncomingBytes(IMessageSocket& socket, std::uint8_t* buffer);

private:
  enum class Status {
    SEEK,
    PARSING_MESSAGE
  };
  
  Status status;
  std::uint32_t parsing_offset;
  std::uint32_t bytes_received;  // amount of data actually received 

  // TODO(cenz): it's not a good idea that the StreamMessageReader should know the number of bytes 
  // occupied by the message length, since it is decided by MessageConveyor and it is a hidden dependency
  uint32_t message_length;    // length of the message currently processed
  bool buffer_should_be_compacted;


  bool processData(std::uint8_t* buffer);
  bool seek(const std::uint8_t* buffer);
  bool parseMessage(const std::uint8_t* buffer);

  // TODO(cenz): find a better name
  // compact buffer to free space for new data
  void compactBuffer(std::uint8_t* buffer);
};


}