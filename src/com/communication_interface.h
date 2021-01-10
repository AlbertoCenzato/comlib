#pragma once

#include "macros.h"

namespace com {

class IMessageSocket
{
public:
  virtual bool connect() = 0;
  virtual void disconnect() = 0;
  virtual bool send(const void* data, uint32_t bytes) = 0;
  virtual uint32_t receive(void* data, uint32_t bytes) = 0;
};



}