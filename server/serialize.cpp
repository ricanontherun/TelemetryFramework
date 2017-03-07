#include <serialize.h>

#include <telemetry.h>
#include <flatbuffers/flatbuffers.h>

#include <response_generated.h>
#include <filesystem_generated.h>
#include <usage_generated.h>

namespace Application {

void SerializeResults(const Telemetry::Results & results, flatbuffers::FlatBufferBuilder & builder)
{
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

  auto string = builder.CreateString("");
  auto flatbuffer_filesystems = builder.CreateVector(filesystems);

  // Create the union with the read response data.
  Telemetry::Buffers::ReadResponseBuilder read_response_builder(builder);
  read_response_builder.add_filesystems(flatbuffer_filesystems);
  auto read_response = read_response_builder.Finish();

  // Create the root response object.
  Telemetry::Buffers::ResponseBuilder response_builder(builder);

  response_builder.add_data_type(Telemetry::Buffers::ResponseData_ReadResponse);
  response_builder.add_data(read_response.Union());
  response_builder.add_success(true);
  response_builder.add_message(string);

  auto response = response_builder.Finish();
  builder.Finish(response);
}

}
