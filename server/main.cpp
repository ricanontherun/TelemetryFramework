#include "../vendor/zmq.hpp"

#include <iostream>
#include <cstdlib>
#include <unistd.h>

int main() {

  zmq::context_t context(1);
  zmq::socket_t socket(context, ZMQ_PULL);

  socket.bind("tcp://*:5555");

  zmq::socket_t replier(context, ZMQ_PUSH);
  replier.bind("tcp://*:5556");

  std::cout << "Server initialized, waiting for connections...\n";

  while (true) {
    zmq::message_t request;
    socket.recv(&request);

    std::cout << "Received request from client...collecting data\n";

    sleep(1);

    zmq::message_t reply(5);
    memcpy(reply.data(), "world", 5);
    replier.send(reply);

    std::cout << "Sent data to server\n";
  }

  return 0;
}