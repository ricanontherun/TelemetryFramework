#include "../vendor/zmq.hpp"

#include <zmq_helpers.h>

#include <iostream>
#include <cstdlib>
#include <unistd.h>

int main() {

  zmq::context_t context(1);

  zmq::socket_t receiver(context, ZMQ_PULL);
  receiver.bind("tcp://*:5555");

  zmq::socket_t replier(context, ZMQ_PUSH);
  replier.bind("tcp://*:5556");

  // TODO: Add in message filtering.
  // TODO: Initialize signal handlers. We never want this process to leave tcp ports bound.


  while (true) {
    std::string message_str;
    zmq::message_t request;
    receiver.recv(&request);

    zmq_extract_message(request, message_str);

    std::cout << "Received: " << message_str << "\n";

    sleep(1);

    zmq::message_t reply(5);
    memcpy(reply.data(), "world", 5);
    replier.send(reply);
  }

  return 0;
}