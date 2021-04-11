#pragma once

#include "messages.h"

namespace com::msg {

struct @className : public IMessage {
  @memberVariables	
  	
  @className() = default;	
  @className(@constructorParams);	
  	
  uint32_t getSize() const override;	
  MessageType getMessageType() const override;	
  uint8_t* serialize(uint8_t* buffer) const override;	
		
  static stdx::UPtr<IMessage> deserialize(const uint8_t* data, const uint8_t** new_buffer_ptr);	
  static MessageType type();	
		
private:	
  inline static DeserializationRegister<Move> reg{"@className"};	
};

}

