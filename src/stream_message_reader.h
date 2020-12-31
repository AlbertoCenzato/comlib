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
  std::size_t message_length;    // length of the message currently processed
  std::uint32_t message_bytes_to_process;
  
  bool processData(std::uint8_t* buffer);
  bool seek(const std::uint8_t* buffer);
  bool parseMessage(const std::uint8_t* buffer);

  // TODO(cenz): find a better name
  void compactBuffer(std::uint8_t* buffer);
};


}