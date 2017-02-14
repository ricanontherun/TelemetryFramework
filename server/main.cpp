#include <zmq.hpp>
#include <zmq_helpers.h>

#include <telemetry.h>
#include <flatbuffers/flatbuffers.h>

#include <response_generated.h>
#include <filesystem_generated.h>
#include <usage_generated.h>

#include <iomanip>

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
  zmq::context_t context(1);

  zmq::socket_t receiver(context, ZMQ_PULL);
  receiver.bind("tcp://*:5555");

  zmq::socket_t replier(context, ZMQ_PUSH);
  replier.bind("tcp://*:5556");

  Telemetry::Options options;
  options.resources = Telemetry::Resource::FILESYSTEMS;

  Telemetry::Results results;
  Telemetry::Unit unit(options);

  zmq::message_t request;
  zmq::message_t reply;
  std::string message_str;

  while (true) {
    receiver.recv(&request);

    if ( !zmq_extract_message(request, message_str) ) {
      // send back an error.
      continue;
    }

    std::cout << "Reading data\n";
    unit.Read(results);

    flatbuffers::FlatBufferBuilder builder(1024);

    SerializeResults(results, builder);

    std::uint8_t *pointer = builder.GetBufferPointer();

    if ( !zmq_pack_message(reply, (void *) pointer, builder.GetSize()) ) {
      // send back an error.
      continue;
    }

    std::cout << "Sending data back\n";
    replier.send(reply);
  }

  return EXIT_SUCCESS;
}

