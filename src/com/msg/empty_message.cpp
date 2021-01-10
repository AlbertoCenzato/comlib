#include "empty_message.h"
#include "../serialization.h"

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
