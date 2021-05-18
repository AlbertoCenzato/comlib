#pragma once

#include "../i_message.h"
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

uint8_t* serialize(const @|className|& msg, uint8_t* buffer);

}

