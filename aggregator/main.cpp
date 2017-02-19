#include <zmq.hpp>
#include <zmq_helpers.h>

#include <flatbuffers/flatbuffers.h>
#include <response_generated.h>
#include <request_generated.h>

#include <fstream>
#include <thread>
#include <iostream>
#include <vector>

int main(int argc, char **argv) {
  flatbuffers::FlatBufferBuilder builder(1024);

  auto procedure = Telemetry::Buffers::CreateReadProcedure(builder, Telemetry::Buffers::RESOURCE_MEMORY);

  Telemetry::Buffers::RequestBuilder request_builder(builder);

  request_builder.add_procedure_type(Telemetry::Buffers::Procedure_ReadProcedure);

  request_builder.add_procedure(procedure.Union());

  auto request_offset = request_builder.Finish();

  builder.Finish(request_offset);

  auto thingy = Telemetry::Buffers::GetRequest(builder.GetBufferPointer());

  int type = thingy->procedure_type();

  switch (type) {
    case Telemetry::Buffers::Procedure_ReadProcedure:
      std::cout << "You're using the read procedure\n";
      break;
  }

  return 0;
  std::vector<std::string> addresses;

  addresses.push_back("tcp://localhost:5555");

  std::thread request_thread([&addresses]() {
    std::vector<zmq::socket_t> sockets;
    sockets.reserve(addresses.size());

    zmq::context_t context(1);

    // setupSockets(addresses, sockets);
    // First, create the sockets.
    for ( auto const & address : addresses ) {
      sockets.push_back(zmq::socket_t(context, ZMQ_PUSH));

      try {
        sockets.back().connect(address);
      } catch ( zmq::error_t & exception ) {
        std::cerr << "Failed to connect to '" << address << "', '" << exception.what() << "'\n";
      }
    }

    // Request loop
    std::string message_str;
    zmq::message_t message;

    while (true) {
      std::cout << "Press Enter: ";

      getchar();

      message_str = "Please send me some data!";

      for ( auto & socket : sockets ) {
        zmq_pack_message(message, message_str);
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

    nlohmann::json reply_json;

    while (true) {
      receiver.recv(&reply);

        auto response = Telemetry::Buffers::GetResponse(reply.data());

        std::cout << response->filesystems()->begin()->label()->c_str();

      std::cout << "Received some replies from the server!!!\n";
    }
  });

  request_thread.join();
  aggregate_thread.join();
}
