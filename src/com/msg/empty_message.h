#pragma once


#include "messages.h"

namespace com::msg {

struct EmptyMessage : public IMessage {

	uint32_t getSize() const override;
	MessageType getMessageType() const override;
	uint8_t* serialize(uint8_t* buffer) const override;

	static stdx::UPtr<IMessage> deserialize(const uint8_t* data, const uint8_t** new_data_ptr);

private:
	inline static DeserializationRegister<EmptyMessage> reg{"EmptyMessage"};
};

}
