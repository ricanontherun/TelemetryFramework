#include <zmq.hpp>
#include <zmq_helpers.h>

#include <telemetry.h>

#include <iostream>
#include <unistd.h>

int main() {

  zmq::context_t context(1);

  zmq::socket_t receiver(context, ZMQ_PULL);
  receiver.bind("tcp://*:5555");

  zmq::socket_t replier(context, ZMQ_PUSH);
  replier.bind("tcp://*:5556");

  // Setup the telemetry stuff.
  Telemetry::Options options;
  options.resources = Telemetry::Resource::DISK;
  Telemetry::Unit unit(options);
  std::string telemetry_str;

  while (true) {
    std::string message_str;
    zmq::message_t request;
    receiver.recv(&request);

    zmq_extract_message(request, message_str);

    std::cout << "Received: " << message_str << "\n";

    unit.Read(telemetry_str);

    zmq::message_t reply(telemetry_str.length());
    memcpy(reply.data(), telemetry_str.c_str(), telemetry_str.length());
    replier.send(reply);
  }

  return 0;
}