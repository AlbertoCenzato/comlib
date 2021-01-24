#include "empty_message.h"
#include "../serialization.h"

namespace com::msg {

uint32_t EmptyMessage::getSize() const {
	return uint32_t(0);
}

MessageType EmptyMessage::getMessageType() const {
	return type();
}

uint8_t* EmptyMessage::serialize(uint8_t* buffer) const {
	return buffer;
}

stdx::UPtr<IMessage> EmptyMessage::deserialize(const uint8_t* data, const uint8_t** new_data_ptr) {
	*new_data_ptr = data;
	return new EmptyMessage{};
}

MessageType EmptyMessage::type() {
	return reg.message_type_id;
}

}
