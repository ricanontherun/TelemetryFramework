#include <client.h>

#include <flatbuffers/flatbuffers.h>
#include <response_generated.h>
#include <request_generated.h>

#include <iostream>

namespace Networking
{

Client::Client()
: context(1)
{};

void Client::AddServer(const std::string & address)
{
  // Holy shit emplace_back ftw.
  this->servers.emplace_back(this->context, ZMQ_PUSH);

  this->servers.back().connect(address);
}

void Client::Read(int resources) {
  flatbuffers::FlatBufferBuilder builder(1024);

  this->BuildReadProcedure(builder, resources);

  std::uint8_t *pointer = builder.GetBufferPointer();

  // For each server, send the read thingers.
  // Pack the binary into a request message and send it to the server.
  // This is a great place to implement network instrumentation like request times
  // and unresponsive servers.
//  auto request_pointer = Telemetry::Buffers::GetRequest(builder.GetBufferPointer());
//
//  std::cout << request_pointer->procedure_type() << "\n";
}

void Client::BuildReadProcedure(
    flatbuffers::FlatBufferBuilder & builder,
    int resources
)
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
}
