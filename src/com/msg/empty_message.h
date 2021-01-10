#pragma once

#include "message_types.h"
#include "../serialization.h"

namespace com::msg {

struct EmptyMessage {

};

constexpr uint32_t getSize(const EmptyMessage& message) {
	return uint32_t(0);
}

template <>
constexpr MessageType getMessageType<EmptyMessage>() {
	return MessageType::EMPTY_MESSAGE;
}

}


namespace com {

template <> inline
uint8_t* serialize<msg::EmptyMessage>(const msg::EmptyMessage& msg, uint8_t* buffer) {
	return buffer;
}

template<> inline
const uint8_t* deserialize<msg::EmptyMessage>(const uint8_t* data, msg::EmptyMessage& message) {
	return data;
}

}
