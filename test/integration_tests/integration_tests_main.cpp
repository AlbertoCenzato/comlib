#include "mocks/mock_message_socket.h"

#include <com/communication.h>

#include <thread>
#include <vector>
#include <numeric>
#include <iostream>

void send(com::test::ThreadsafeLoopbackSocket& socket, const std::vector<int>& data);
void receive(com::test::ThreadsafeLoopbackSocket& socket, std::vector<int>& data);

constexpr int QUEUE_SIZE = 111111;

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

  std::cout << "Data received: " << std::endl;
  for (int n : receive_queue) {
    std::cout << n << std::endl;
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
    switch (r % 1) {
    case 0:
      com::MoveMessage move_message;
      move_message.x = n;
      success = conveyor.send(move_message);
      break;
    case 1:
      com::Int32Message int32_message;
      int32_message.value = n;
      success = conveyor.send(int32_message);
      break;
    case 2:
      com::EmptyMessage empty_message;
      success = conveyor.send(empty_message);
      break;
    }

    if (!success) {
      std::cout << "Send error" << std::endl;
    }
    else {
      std::cout << "Sent: " << n << std::endl;
    }
    //std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  conveyor.disconnect();
}

void receive(com::test::ThreadsafeLoopbackSocket& socket, std::vector<int>& data) {
  com::MessageConveyor conveyor{ &socket };
  conveyor.connect();

  int n = -1;
  while (n < (QUEUE_SIZE - 1)) {
    auto message = conveyor.processIncomingMessage();
    if (message) {
      const com::MessageType type = message.value().type;
      std::cout << com::to_string(type) << " received" << std::endl;
      switch (type) {
      case com::MessageType::EMPTY_MESSAGE:
        break;
      case com::MessageType::INT32_MESSAGE:
        n = message.value().message.int32.value;
        std::cout << "value: " << n << std::endl;
        data.push_back(n);
        break;
      case com::MessageType::MOVE_MESSAGE:
        n = message.value().message.move.x;
        std::cout << "x: " << n << std::endl;
        data.push_back(n);
        break;
      }
    }
    else {
      std::cout << "Waiting for message completion" << std::endl;
    }

    //std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  conveyor.disconnect();
}