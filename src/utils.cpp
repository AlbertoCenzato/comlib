#include "utils.h"

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

}

}