#include <zmq.hpp>
#include <zmq_functions.h>
#include <client.h>

#include <fstream>
#include <thread>
#include <iostream>
#include <vector>

#include <response_generated.h>

int main(int argc, char **argv) {
  Networking::Client client;

  client.Read(2);

  return 1;
  std::vector<std::string> addresses;

  addresses.push_back("tcp://localhost:5555");

  std::thread request_thread([&addresses]() {
    std::vector<zmq::socket_t> sockets;
    sockets.reserve(addresses.size());
    zmq::message_t message;

    zmq::context_t context(1);

    // First, create the sockets.
    for ( auto const & address : addresses ) {
      sockets.push_back(zmq::socket_t(context, ZMQ_PUSH));

      try {
        sockets.back().connect(address);
      } catch ( zmq::error_t & exception ) {
        std::cerr << "Failed to connect to '" << address << "', '" << exception.what() << "'\n";
      }
    }

    while (true) {
      std::cout << "Press Enter: ";

      getchar();

      std::string message_str = "Please send me some data!";

      for ( auto & socket : sockets ) {
        ZMQFunctions::pack(message, (void *) message_str.c_str(), message_str.length());
        socket.send(message);
      }
    }
  });

  std::thread aggregate_thread([]() {
    zmq::context_t context(1);
    zmq::socket_t receiver(context, ZMQ_PULL);

    receiver.connect("tcp://localhost:5556");

    zmq::message_t reply;
    std::string reply_str;

    while (true) {
      receiver.recv(&reply);

      auto response = Telemetry::Buffers::GetResponse(reply.data());

      std::cout << "Filesystem Label " << response->filesystems()->begin()->label()->c_str() << "\n";
    }
  });

  request_thread.join();
  aggregate_thread.join();
}
