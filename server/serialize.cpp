#include <serialize.h>

#include <telemetry.h>
#include <flatbuffers/flatbuffers.h>

#include <response_generated.h>
#include <filesystem_generated.h>
#include <usage_generated.h>

namespace Application {

void SerializeResults(const Telemetry::Results & results, flatbuffers::FlatBufferBuilder & builder)
{
  // flatbuffers must be constructed using a depth first approach.

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

}
