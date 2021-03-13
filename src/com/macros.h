#pragma once

#if _MSC_VER
#define _restrict __restrict
#elif ARDUINO_AVR_UNO
#define _restrict
#else
#define _restrict __restrict__
#endif

#if _MSC_VER
#define LITTLE_ENDIAN true
#elif ARDUINO_AVR_UNO
#define LITTLE_ENDIAN true
#else 
#undef LITTLE_ENDIAN  // TODO(cenz): fix this
#define LITTLE_ENDIAN true
#endif

#if ARDUINO_AVR_UNO
#include <Arduino.h>

constexpr int LED = 13;

inline void blink(int ms) {
  digitalWrite(LED, HIGH);
  delay(ms);
  digitalWrite(LED, LOW);
  delay(ms);
}

inline void toBlinkCoding(uint32_t value) {
  for (uint32_t i = 0; i < value; ++i) {
    blink(500);
  }
}

#else
#include <cstdint>
#include <cstddef>
#include <string>

using String = std::string;

#endif