#include <communication.h>

#include "mocks/mock_message_socket.h"
#include <thread>
#include <vector>
#include <numeric>
#include <iostream>

using Buffer = std::queue<std::uint8_t>;

void send(Buffer* send_queue, Buffer* receive_queue, const std::vector<int>& data);
void receive(Buffer* send_queue, Buffer* receive_queue, std::vector<int>& data);

int main() {
  std::vector<int> send_queue, receive_queue;
  send_queue.resize(501);
  std::iota(send_queue.begin(), send_queue.end(), 0);

  {
    Buffer buffer_a, buffer_b;
    std::thread send_thread{ send, &buffer_a, &buffer_b, std::cref(send_queue) };
    std::thread receive_thread{ receive, &buffer_b, &buffer_a, std::ref(receive_queue) };

    send_thread.join();
    receive_thread.join();
  }

  std::cout << "Data received: " << std::endl;
  for (int n : receive_queue) {
    std::cout << n << std::endl;
  }

  return 0;
}

void send(Buffer* send_queue, Buffer* receive_queue, const std::vector<int>& data) {
  com::test::MockMessageSocket socket{ send_queue, receive_queue };
  com::MessageConveyor conveyor{ &socket };

  conveyor.connect();
  
  for (int n : data) {
    com::MoveMessage m;
    m.x = n;
    bool success = conveyor.send(m);
    if (!success) {
      std::cout << "Send error" << std::endl;
    }
    else {
      std::cout << "Sent: " << n << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  conveyor.disconnect();
}

void receive(Buffer* send_queue, Buffer* receive_queue, std::vector<int>& data) {
  com::test::MockMessageSocket socket{ send_queue, receive_queue };
  com::MessageConveyor conveyor{ &socket };

  conveyor.connect();

  int n = -1;
  while (n < 500) {
    auto message = conveyor.processIncomingMessage();
    if (message) {
      n = message.value().message.move.x;
      data.push_back(n);
      std::cout << "received:" << n << std::endl;
    }
    else {
      std::cout << "Waiting for message completion" << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  conveyor.disconnect();
}