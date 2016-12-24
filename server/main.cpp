#include "../vendor/zmq.hpp"

#include <zmq_helpers.h>

#include <iostream>
#include <cstdlib>
#include <unistd.h>

int main() {

  zmq::context_t context(1);

  zmq::socket_t receiver(context, ZMQ_PULL);
  receiver.bind("tcp://*:5555");

  std::cout << "Receiver: Bound to *:5555\n";

  zmq::socket_t replier(context, ZMQ_PUSH);
  replier.bind("tcp://*:5556");

  std::cout << "Replier: Bound to *:5556\n";

  std::cout << "Server initialized. Waiting for ventilator requests\n";

  // TODO: Add in message filtering.
  // TODO: Initialize signal handlers. We never want this process to leave tcp ports bound.

  std::string message_str;

  while (true) {
    zmq::message_t request;
    receiver.recv(&request);

    zmq_extract_message(request, message_str);

    std::cout << "Received: " << message_str << "\n";

    sleep(1);

    zmq::message_t reply(5);
    memcpy(reply.data(), "world", 5);
    replier.send(reply);

    std::cout << "Sent data to server\n";
  }

  return 0;
}