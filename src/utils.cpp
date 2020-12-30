#include "utils.h"
#include "messages.h"
#include <cstdint>

namespace com {

namespace utils {


void* reverseMemcpy(void* _restrict dest, const void* _restrict source, size_t len) {
  std::uint8_t* dest_u8 = reinterpret_cast<std::uint8_t*>(dest);
  const std::uint8_t* source_u8 = reinterpret_cast<const std::uint8_t*>(source);
  for (size_t i = 0; i < len; ++i)
    dest_u8[i] = source_u8[len - i - 1];
  return dest;
}

void littleEndianCopy(void* dest, const void* src, size_t len)
{
  if _constexpr(isLittleEndian())
    memcpy(dest, src, len);
  else
    reverseMemcpy(dest, src, len);
}

void bigEndianCopy(void* dest, const void* src, size_t len)
{
  if _constexpr(isLittleEndian())
    reverseMemcpy(dest, src, len);
  else
    memcpy(dest, src, len);
}

void profileTime(const char* name, std::function<void()> func) {
  auto start = std::chrono::steady_clock::now();
  func();
  auto end = std::chrono::steady_clock::now();
  auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
  std::cout << "Profiling " << name << ": " << elapsed << "ms" << std::endl;
}

template <> inline
std::uint8_t* serialize<MessageType>(const MessageType& type, std::uint8_t* buffer) {
  return serialize(static_cast<std::int16_t>(type), buffer);
}

template <> inline
std::uint8_t* serialize<float>(const float& value, std::uint8_t* buffer) {
  memcpy(buffer, &value, sizeof(float));
  return buffer + sizeof(float);
}


template<> inline
const std::uint8_t* deserialize<float>(const std::uint8_t* data, float& value) {
  value = *reinterpret_cast<const float*>(data);
  return data + sizeof(float);
}

template<> inline
const std::uint8_t* deserialize<MessageType>(const std::uint8_t* data, MessageType& value) {
  return deserialize(data, reinterpret_cast<std::int16_t&>(value));
}


void shiftBytesLeft(void* array, size_t length, size_t shift) {
//  memmove(array - shift, array, length);
}
void shiftBytesRight(void* array, size_t length, size_t shift) {
//  memmove(array + shift, array, length);
}


}

}