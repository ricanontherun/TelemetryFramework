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

Server::Server(int port)
: port(port),
  context(1),
  client_socket(context, ZMQ_ROUTER),
  worker_socket(context, ZMQ_DEALER),
  unit(Telemetry::Unit()),
  num_threads(std::thread::hardware_concurrency())
{};

bool Server::Init()
{
  try {
    // Use configured port.
    this->client_socket.bind("tcp://*:5555");
    this->worker_socket.bind("inproc://workers");

    return true;
  } catch ( zmq::error_t & e ) {
    std::cerr << "Server::Init, " << e.what() << "\n";

    return false;
  }
}

void Server::WorkerThread()
{
  // Create a vanilla rep socket
  zmq::socket_t socket(this->context, ZMQ_REP);

  socket.connect("inproc://workers");

  zmq::message_t request;
  zmq::message_t reply;

  std::string reply_string = "Roman!!";

  std::cout << "Waiting for client\n";

  while (true) {
    socket.recv(&request);

    std::cout << std::this_thread::get_id() << "\n";

    ZMQFunctions::pack(reply, (void *) reply_string.c_str(), reply_string.length());

    socket.send(reply);
  }
}

bool Server::HandleRequest(const Request & request, Reply & reply)
{
  // Unpack the buffer
  std::uint8_t *buffer;
  std::size_t buffer_length;
  ZMQFunctions::extract(request.request, (void **)(&buffer), buffer_length);

  flatbuffers::Verifier verifer(buffer, buffer_length);
  bool ok = Telemetry::Buffers::VerifyRequestBuffer(verifer);

  std::cout << "Is the buffer a valid request buffer?: " << buffer_length << "\n";

  Telemetry::Results results;

  Telemetry::Options options;
  options.resources = Telemetry::Resource::FILESYSTEMS;

  unit.Read(options, results);

  flatbuffers::FlatBufferBuilder builder(1024);

  // TODO: Error checking
  Application::SerializeResults(results, builder);

  ZMQFunctions::pack(reply.reply, (void *) builder.GetBufferPointer(), builder.GetSize());

  return true;
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
