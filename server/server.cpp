#include <server.h>

#include <telemetry.h>
#include <serialize.h>
#include <flatbuffers/flatbuffers.h>

#include <cstdint>
#include <unistd.h>

namespace Networking
{

Server::Server()
: context(1),
  client_socket(context, ZMQ_ROUTER),
  worker_socket(context, ZMQ_DEALER),
  num_threads(std::thread::hardware_concurrency())
{};

bool Server::Init(const Options & options)
{
  try {
    std::string server_address = "tcp://*:" + std::to_string(options.port);

    this->client_socket.bind(server_address);
    this->worker_socket.bind("inproc://workers");

    return true;
  } catch ( zmq::error_t & e ) {
    std::cerr << "Server::Init, " << e.what() << "\n";

    return false;
  }
}

void Server::WorkerThread()
{
  zmq::socket_t socket(this->context, ZMQ_REP);

  socket.connect("inproc://workers");

  zmq::message_t request;
  zmq::message_t reply;

  while (true) {
    socket.recv(&request);

    this->HandleRequest(request, reply);

    socket.send(reply);
  }
}

bool Server::HandleRequest(const zmq::message_t & request, zmq::message_t & reply)
{
  std::uint8_t *buffer;
  std::size_t buffer_length;

  // Extract the buffer from the message.
  ZMQFunctions::extract(request, (void **)(&buffer), buffer_length);

  // Validate the request. We want to make sure it's a valid request.
  if ( !this->ValidateRequestBuffer(buffer, buffer_length) ) {
    // this->BuildErrorReply();
    return false;
  }

  // Construct a request object from the buffer.
  auto client_request = Telemetry::Buffers::GetRequest(buffer);

  switch ( client_request->procedure_type() ) {
    case 0:
      break;
    case Telemetry::Buffers::Procedure_ReadProcedure:
      auto procedure = static_cast<const Telemetry::Buffers::ReadProcedure *>(
          client_request->procedure()
      );

      this->PerformRead(procedure, reply);

      break;
  }

  return true;
}

void Server::PerformRead(
  const Telemetry::Buffers::ReadProcedure * procedure,
  zmq::message_t & reply
)
const
{
  Telemetry::Results results;
  Telemetry::Unit unit;
  Telemetry::Options options;

  options.resources = procedure->Resource();

  unit.Read(options, results);

  // Serialize the results into a response object.
  flatbuffers::FlatBufferBuilder builder(1024);
  Application::SerializeResults(results, builder);

  // Pack the response into the reply message.
  ZMQFunctions::pack(reply, (void *) builder.GetBufferPointer(), builder.GetSize());
}

bool Server::ValidateRequestBuffer(
    std::uint8_t * buffer_pointer,
    const std::size_t & buffer_length
) const
{
  flatbuffers::Verifier verifer(buffer_pointer, buffer_length);

  return Telemetry::Buffers::VerifyRequestBuffer(verifer);
}

void Server::Accept()
{
  this->workers_threads.reserve(this->num_threads);

  for ( std::uint8_t i = 0; i < this->num_threads; ++i ) {
    this->workers_threads.push_back(
        std::thread(&Server::WorkerThread, this)
    );
  }

  zmq::proxy(this->client_socket, this->worker_socket, NULL);
}

}
