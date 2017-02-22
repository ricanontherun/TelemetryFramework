#include <zmq.hpp>
#include <zmq_helpers.h>

#include <flatbuffers/flatbuffers.h>
#include <response_generated.h>
#include <request_generated.h>

#include <fstream>
#include <thread>
#include <iostream>
#include <vector>

// We need an interface for the remote procedures. They can get called via this
// TelemetryClient class, which will handle marshalling requests and unmarshalling
// replies and all that.
class TelemetryClient
{
  public:
    TelemetryClient() {}

    void Read(int resources) {
      flatbuffers::FlatBufferBuilder builder(1024);

      this->BuildReadProcedure(builder, resources);

      std::uint8_t *pointer = builder.GetBufferPointer();

      // Pack the binary into a request message and send it to the server.
      // This is a great place to implement network instrumentation like request times
      // and unresponsive servers.
      auto request_pointer = Telemetry::Buffers::GetRequest(builder.GetBufferPointer());

      std::cout << request_pointer->procedure_type() << "\n";
    }

  private:
    void BuildReadProcedure(flatbuffers::FlatBufferBuilder & builder, int resources)
    {
      // Super important to create the nested elements with a depth-first approach.
      auto procedure = Telemetry::Buffers::CreateReadProcedure(
          builder,
          static_cast<Telemetry::Buffers::RESOURCE>(resources)
      );

      Telemetry::Buffers::RequestBuilder request_builder(builder);

      // Add the already created elements.
      request_builder.add_procedure_type(Telemetry::Buffers::Procedure_ReadProcedure);
      request_builder.add_procedure(procedure.Union());

      auto request_buffer_offset = request_builder.Finish();

      builder.Finish(request_buffer_offset);
    }
};

int main(int argc, char **argv) {
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

    TelemetryClient client;

    // client.AddServer();

    client.Read(Telemetry::Buffers::RESOURCE::RESOURCE_FILESYSTEMS);

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
