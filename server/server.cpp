#include <server.h>

#include <telemetry.h>
#include <serialize.h>
#include <flatbuffers/flatbuffers.h>
#include <request_generated.h>
#include <response_generated.h>
#include <filesystem_generated.h>
#include <usage_generated.h>

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
  // Unpack the buffer
  std::uint8_t *buffer;
  std::size_t buffer_length;
  ZMQFunctions::extract(request, (void **)(&buffer), buffer_length);

  if ( !this->ValidateRequestBuffer(buffer, buffer_length) ) {
    // this->BuildErrorReply();
    return false;
  }

  auto client_request = Telemetry::Buffers::GetRequest(buffer);

  std::cout << client_request->procedure_type() << "\n";
  // If it's not a valid buffer, return an error with an approprirate string.
  std::cout << "Request is valid\n";

  Telemetry::Results results;
  Telemetry::Unit unit;

  Telemetry::Options options;
  options.resources = Telemetry::Resource::FILESYSTEMS;

  unit.Read(options, results);

  flatbuffers::FlatBufferBuilder builder(1024);

  // TODO: Error checking
  Application::SerializeResults(results, builder);

  ZMQFunctions::pack(reply, (void *) builder.GetBufferPointer(), builder.GetSize());

  return true;
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
