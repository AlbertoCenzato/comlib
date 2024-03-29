#include "com/communication.h"

namespace {

  void swap(Message& a, Message& b) noexcept {
    using com::stdx::swap;
    swap(a.message_type_id, b.message_type_id);
    swap(a.message, b.message);
  }

}

namespace com {

  Message::Message() : message_type_id(0), message() {}

  Message::Message(msg::MessageType message_type_id, stdx::UPtr<msg::IMessage>&& message)
    : message_type_id(message_type_id), message(stdx::move(message)) {}

  Message::Message(Message&& mess) noexcept : Message() { 
    swap(*this, mess); 
  }

  Message& Message::operator=(Message mess) { 
    swap(*this, mess); 
    return *this; 
  }

}