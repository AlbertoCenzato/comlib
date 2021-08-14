#pragma once

#include "com/i_message.h"
#include "com/serialization.h"
@messageDependenciesHeaders

namespace com::msg {

struct @className : public IMessage {
  @memberVariables	
  	
  @defaultConstructor	
  @className(@constructorParams);	
  	
  uint32_t getSize() const override;	
  MessageType getMessageType() const override;	
  uint8_t* serialize(uint8_t* buffer) const override;	
		
  static stdx::UPtr<IMessage> deserialize(const uint8_t* data, const uint8_t** new_buffer_ptr);	
  static MessageType type();	
		
private:	
  inline static DeserializationRegister<@|className|> reg{"@className"};	
};

}


namespace com {

template <>
uint8_t* serialize<msg::@|className|>(const msg::@|className|& msg, uint8_t* buffer);

template <>
const uint8_t* deserialize<msg::@|className|>(const uint8_t* buffer, msg::@|className|& msg);

}
