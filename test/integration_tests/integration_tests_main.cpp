#include "mocks/mock_message_socket.h"

#include <com/communication.h>
#include <com/utils.h>
#include <com/msg/empty_message.h>
#include <com/msg/int32_message.h>
#include <com/msg/move_message.h>

#include <thread>
#include <vector>
#include <numeric>
#include <iostream>

void send(com::test::ThreadsafeLoopbackSocket& socket, const std::vector<int>& data);
void receive(com::test::ThreadsafeLoopbackSocket& socket, std::vector<int>& data);

constexpr int QUEUE_SIZE = 1000000;

int main() {
  std::vector<int> send_queue, receive_queue;
  send_queue.resize(QUEUE_SIZE);
  std::iota(send_queue.begin(), send_queue.end(), 0);

  {
    com::test::ThreadsafeLoopbackSocket socket;
    std::thread send_thread{ send, std::ref(socket), std::cref(send_queue) };
    std::thread receive_thread{ receive, std::ref(socket), std::ref(receive_queue) };

    send_thread.join();
    receive_thread.join();
  }

  std::cout << "Test finished." << std::endl;
  bool passed = true;
  for (size_t i = 0; i < send_queue.size() - 1; i++) {
    if (send_queue[i] != receive_queue[i]) {
      passed = false;
      break;
    }
  }

  if (passed) {
    std::cout << "Test passed." << std::endl;
  }
  else {
    std::cout << "Test failed." << std::endl;
  }

  int x;
  std::cin >> x;

  return 0;
}

void send(com::test::ThreadsafeLoopbackSocket& socket, const std::vector<int>& data) {
  com::MessageConveyor conveyor{ &socket };
  conveyor.connect();
  
  for (int n : data) {
    bool success = false;
    int r = std::rand();
    switch (r % 2) {
    case 0: {
      com::msg::MoveMessage move_message;
      move_message.x = n;
      success = conveyor.send(move_message);
      break;
    }
    case 1: {
      com::msg::Int32Message int32_message;
      int32_message.value = n;
      success = conveyor.send(int32_message);
      break;
    }
    case 2: {
      com::msg::EmptyMessage empty_message;
      success = conveyor.send(empty_message);
      break;
    }
    }

    /*
    if (!success) {
      std::cout << "Send error" << std::endl;
    }
    else if (n % 1000 == 0) {
      std::cout << "Sent: " << n << std::endl;
    }
    */
  }

  conveyor.disconnect();
}


void callback(const msg::IMessage& message) {

}

void receive(com::test::ThreadsafeLoopbackSocket& socket, std::vector<int>& data) {
  com::MessageConveyor conveyor{ &socket };
  conveyor.connect();

  int n = -1;
  com::Message mess;
  
  conveyor.registerCallback<msg::Int32Message>(callback);

  /*
  while (n < (QUEUE_SIZE - 1)) {
    mess = conveyor.processIncomingMessage();
    if (mess.message) {
      const com::msg::MessageType type = static_cast<com::msg::MessageType>(mess.message_type_id);
      switch (type) {
      case com::msg::MessageType::EMPTY_MESSAGE:
        break;
      case com::msg::MessageType::INT32_MESSAGE: {
        auto message = dynamic_cast<const com::msg::Int32Message&>(*mess.message);
        n = message.value;
        data.push_back(n);
        break;
      }
      case com::msg::MessageType::MOVE_MESSAGE: {
        auto message = dynamic_cast<const com::msg::MoveMessage&>(*mess.message);
        n = message.x;
        data.push_back(n);
        break;
      }
      }
    }
  }
  */

  conveyor.disconnect();
}