#include "utils.h"
#include "messages.h"

#include <cstdint>

namespace com {

namespace utils {


void* reverseMemcpy(void* _restrict dest, const void* _restrict source, size_t len) {
  uint8_t* dest_u8 = reinterpret_cast<uint8_t*>(dest);
  const uint8_t* source_u8 = reinterpret_cast<const uint8_t*>(source);
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

void shiftBytesLeft(uint8_t* array, size_t length, size_t shift) {
  memmove(array - shift, array, length);
}
void shiftBytesRight(uint8_t* array, size_t length, size_t shift) {
  memmove(array + shift, array, length);
}


}

}