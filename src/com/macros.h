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
#else
#include <cstdint>
#include <cstddef>
#endif