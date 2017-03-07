#include <zmq.hpp>
#include <zmq_functions.h>
#include <client.h>

#include <fstream>
#include <thread>
#include <iostream>
#include <vector>

#include <request_generated.h>
#include <response_generated.h>

int main(int argc, char **argv) {
  zmq::context_t context(1);
  zmq::socket_t socket(context, ZMQ_REQ);
  socket.connect("tcp://localhost:5555");

  flatbuffers::FlatBufferBuilder builder(1024);

  // Create procedure union
  auto procedure = Telemetry::Buffers::CreateReadProcedure(
      builder,
      Telemetry::Buffers::RESOURCE_MEMORY
  );

  // Create the main request object
  Telemetry::Buffers::RequestBuilder request_builder(builder);

  // Add the procudure type
  request_builder.add_procedure_type(Telemetry::Buffers::Procedure_ReadProcedure);

  // Insert the union
  request_builder.add_procedure(procedure.Union());

  // Finish building the request.
  auto request_buffer_offset = request_builder.Finish();

  // Finish the building the entire buffer.
  builder.Finish(request_buffer_offset);

  // Send that shit on the wire.
  zmq::message_t request;
  zmq::message_t reply;

  ZMQFunctions::pack(request, builder.GetBufferPointer(), builder.GetSize());

  socket.send(request);
  socket.recv(&reply);

  std::uint8_t * buffer_pointer;
  std::size_t buffer_length;
  ZMQFunctions::extract(reply, (void **) (&buffer_pointer), buffer_length);

  flatbuffers::Verifier verifer(buffer_pointer, buffer_length);
  auto valid_Response = Telemetry::Buffers::VerifyResponseBuffer(verifer);

  if ( valid_Response ) {
    auto response = Telemetry::Buffers::GetResponse(buffer_pointer);
    std::cout << response->success() << "\n";
  }

  free(buffer_pointer);


  return 0;
}
