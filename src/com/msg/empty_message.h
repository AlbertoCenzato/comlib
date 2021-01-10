#pragma once

#include "message_types.h"

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
