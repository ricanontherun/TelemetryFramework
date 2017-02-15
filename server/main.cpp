#include <zmq.hpp>
#include <zmq_helpers.h>

#include <telemetry.h>
#include <flatbuffers/flatbuffers.h>

#include <response_generated.h>
#include <filesystem_generated.h>
#include <usage_generated.h>

#include <iostream>
#include <iomanip>
#include <unistd.h>

void SerializeResults(const Telemetry::Results & results, flatbuffers::FlatBufferBuilder & builder);

namespace Networking
{
  class Request
  {
    friend class Server;

    private:
      zmq::message_t request;

    public:
      Request() {}
  };

  class Reply
  {
    friend class Server;

    private:
      zmq::message_t reply;
    public:
      Reply() {};
  };

  class Server
  {
    public:
      Server()
      : context(1), request_socket(context, ZMQ_PULL), reply_socket(context, ZMQ_PUSH), unit(Telemetry::Unit()) {};

      bool Init()
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

      bool WaitForClient(Request & request)
      {
        this->request_socket.recv(&(request.request));
      }

      bool HandleRequest(const Request & request, Reply & reply)
      {
        // Unmarshal the request, figure out what procedure is being request
        // with what arugments.
        Telemetry::Results results;

        Telemetry::Options options;
        options.resources = Telemetry::Resource::FILESYSTEMS;

        unit.Read(options, results);

        flatbuffers::FlatBufferBuilder builder(1024);

        // TODO: Error checking
        SerializeResults(results, builder);

        zmq_pack_message(reply.reply, (void *) builder.GetBufferPointer(), builder.GetSize());

        return true;
      }

      bool SendReply(Reply & reply)
      {
        this->reply_socket.send(reply.reply);

        return true;
      }

    private:
      zmq::context_t context;
      zmq::socket_t request_socket;
      zmq::socket_t reply_socket;

      Telemetry::Unit unit;
  };
}

void SerializeResults(const Telemetry::Results & results, flatbuffers::FlatBufferBuilder & builder)
{

  // Create all of the data structures which reside inside a response buffer.
  // Then, create the response builder, add the inside structures, finish().

  // vector of filesystem structs.
  std::vector<flatbuffers::Offset<Telemetry::Buffers::Filesystem>> filesystems;

  // Insert
  Telemetry::Core::Sys::FileSystemIterators fsit = results.GetFilesystemIterators();

  for (auto it = fsit.first; it != fsit.second; it++)
  {
    auto label = builder.CreateString(it->GetLabel());

    Telemetry::Buffers::Usage size(
        it->GetSize(),          // Actual
        it->GetRelativeSize()   // Relative
        );

    Telemetry::Buffers::Usage used(
        it->GetUsed(),
        it->GetRelativeUsed()
        );

    Telemetry::Buffers::Usage available(
        it->GetAvailable(),
        it->GetRelativeAvailable()
        );

    auto filesystem = Telemetry::Buffers::CreateFilesystem(
        builder,
        label,
        &size,
        &used,
        &available
        );

    filesystems.push_back(filesystem);
  }

  // Serialize the std::vector into a flatbuffers vector.
  auto flatbuffer_filesystems = builder.CreateVector(filesystems);

  Telemetry::Buffers::ResponseBuilder response_builder(builder);

  // Add the filesystems, memory, processes etc.
  response_builder.add_filesystems(flatbuffer_filesystems);

  auto response = response_builder.Finish();

  // Wrap that builder up.
  builder.Finish(response);
}

int main() {
  Networking::Server server;

  server.Init();

  Networking::Request request;
  Networking::Reply reply;

  while (true) {
    server.WaitForClient(request);

    // Next we need to parse out the request command and the arguments and all that.
    server.HandleRequest(request, reply);

    server.SendReply(reply);
  }
}

