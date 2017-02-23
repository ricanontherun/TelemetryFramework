#include <server.h>

#include <telemetry.h>
#include <serialize.h>
#include <flatbuffers/flatbuffers.h>
#include <request_generated.h>
#include <response_generated.h>
#include <filesystem_generated.h>
#include <usage_generated.h>

#include <cstdint>

namespace Networking
{

Server::Server()
: context(1), request_socket(context, ZMQ_PULL), reply_socket(context, ZMQ_PUSH), unit(Telemetry::Unit())
{};

bool Server::Init()
{
  try {
    this->request_socket.bind("tcp://*:5555");
    this->reply_socket.bind("tcp://*:5556");

    return true;
  } catch ( zmq::error_t & e ) {
    std::cerr << "Server::Init, " << e.what() << "\n";

    return false;
  }
}

void Server::WaitForClient(Request & request)
{
  this->request_socket.recv(&(request.request));
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

bool Server::SendReply(Reply & reply)
{
  this->reply_socket.send(reply.reply);

  return true;
}

}
