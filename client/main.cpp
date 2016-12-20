#include "../vendor/zmq.hpp"

#include <iostream>
#include <cstdio>
#include <cstdlib>

int main() {
  zmq::context_t context(1);
  zmq::socket_t sender(context, ZMQ_REQ);

  sender.connect("tcp://localhost:5555");

  zmq::message_t message(5);
  memcpy(message.data(), "hello", 5);

  while (true) {
    std::cout << "Press enter to send the next request\n";
    getchar();

    std::cout << "Sending '" << (char *) message.data() << "' to server\n";

    sender.send(message);

    // Wait for the reply
    std::cout << "Waiting for the json reply...\n";
    zmq::message_t reply;
    sender.recv(&reply);
    std::cout << "Received something back from server!\n";
  }
}