#include "mocks/mock_message_socket.h"

#include <com/communication.h>
#include <com/utils.h>
#include <com/msg/empty_message.h>
#include <com/msg/Int32.h>
#include <com/msg/Move.h>

#include <thread>
#include <vector>
#include <numeric>
#include <iostream>
#include <functional>

void send(com::test::ThreadsafeLoopbackSocket& socket, const std::vector<int>& data);
void receive(com::test::ThreadsafeLoopbackSocket& socket);

constexpr int QUEUE_SIZE = 1000000;

std::vector<int> receive_queue;

int main() {
  std::vector<int> send_queue;
  send_queue.resize(QUEUE_SIZE);
  std::iota(send_queue.begin(), send_queue.end(), 0);

  {
    com::test::ThreadsafeLoopbackSocket socket;
    std::thread send_thread{ send, std::ref(socket), std::cref(send_queue) };
    std::thread receive_thread{ receive, std::ref(socket) };

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
      com::msg::Move move_message;
      move_message.x = n;
      success = conveyor.send(move_message);
      break;
    }
    case 1: {
      com::msg::Int32 int32_message;
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
  }

  conveyor.disconnect();
}


int messages_received = 0;

void callbackEmpty(const msg::IMessage& message) {
  std::cout << "Empty callback called!" << std::endl;
  messages_received++;
}

void callbackInt32(const msg::IMessage& message) {
  const auto& int32_message = dynamic_cast<const com::msg::Int32&>(message);
  receive_queue.push_back(int32_message.value);
  messages_received++;
}

void callbackMove(const msg::IMessage& message) {
  const auto& move_message = dynamic_cast<const com::msg::Move&>(message);
  receive_queue.push_back(move_message.x);
  messages_received++;
}


void receive(com::test::ThreadsafeLoopbackSocket& socket) {
  com::MessageConveyor conveyor{ &socket };
  conveyor.connect();
    
  conveyor.registerCallback<msg::EmptyMessage>(callbackEmpty);
  conveyor.registerCallback<msg::Int32>(callbackInt32);
  conveyor.registerCallback<msg::Move>(callbackMove);

  while (messages_received < (QUEUE_SIZE - 1)) {
    conveyor.processIncomingMessage();
    //std::this_thread::sleep_for(std::chrono::milliseconds{10});
  }

  conveyor.disconnect();
}