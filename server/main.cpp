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

  zmq::message_t request;
  zmq::message_t reply;
  std::string message_str;

  while (true) {
    receiver.recv(&request);

    if ( !zmq_extract_message(request, message_str) ) {
      // send back an error.
      continue;
    }

    // TODO: What do we look for in the message?

    // Read the system information into a string.
    unit.Read(telemetry_str);

    if ( !zmq_pack_message(reply, telemetry_str) ) {
      // send back an error.
      continue;
    }

    replier.send(reply);
  }

  return EXIT_SUCCESS;
}