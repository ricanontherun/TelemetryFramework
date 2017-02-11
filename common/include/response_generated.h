// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_RESPONSE_TELEMETRY_BUFFERS_H_
#define FLATBUFFERS_GENERATED_RESPONSE_TELEMETRY_BUFFERS_H_

#include "flatbuffers/flatbuffers.h"

#include "filesystem_generated.h"
#include "memory_generated.h"
#include "usage_generated.h"

namespace Telemetry {
namespace Buffers {

struct Response;

struct Response FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_FILESYSTEMS = 4,
    VT_MEMORY = 6
  };
  const flatbuffers::Vector<flatbuffers::Offset<Telemetry::Buffers::Filesystem>> *filesystems() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Telemetry::Buffers::Filesystem>> *>(VT_FILESYSTEMS);
  }
  const Telemetry::Buffers::Memory *memory() const {
    return GetPointer<const Telemetry::Buffers::Memory *>(VT_MEMORY);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_FILESYSTEMS) &&
           verifier.Verify(filesystems()) &&
           verifier.VerifyVectorOfTables(filesystems()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_MEMORY) &&
           verifier.VerifyTable(memory()) &&
           verifier.EndTable();
  }
};

struct ResponseBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_filesystems(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Telemetry::Buffers::Filesystem>>> filesystems) {
    fbb_.AddOffset(Response::VT_FILESYSTEMS, filesystems);
  }
  void add_memory(flatbuffers::Offset<Telemetry::Buffers::Memory> memory) {
    fbb_.AddOffset(Response::VT_MEMORY, memory);
  }
  ResponseBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ResponseBuilder &operator=(const ResponseBuilder &);
  flatbuffers::Offset<Response> Finish() {
    const auto end = fbb_.EndTable(start_, 2);
    auto o = flatbuffers::Offset<Response>(end);
    return o;
  }
};

inline flatbuffers::Offset<Response> CreateResponse(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Telemetry::Buffers::Filesystem>>> filesystems = 0,
    flatbuffers::Offset<Telemetry::Buffers::Memory> memory = 0) {
  ResponseBuilder builder_(_fbb);
  builder_.add_memory(memory);
  builder_.add_filesystems(filesystems);
  return builder_.Finish();
}

inline flatbuffers::Offset<Response> CreateResponseDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<flatbuffers::Offset<Telemetry::Buffers::Filesystem>> *filesystems = nullptr,
    flatbuffers::Offset<Telemetry::Buffers::Memory> memory = 0) {
  return CreateResponse(
      _fbb,
      filesystems ? _fbb.CreateVector<flatbuffers::Offset<Telemetry::Buffers::Filesystem>>(*filesystems) : 0,
      memory);
}

inline const Telemetry::Buffers::Response *GetResponse(const void *buf) {
  return flatbuffers::GetRoot<Telemetry::Buffers::Response>(buf);
}

inline bool VerifyResponseBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<Telemetry::Buffers::Response>(nullptr);
}

inline void FinishResponseBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<Telemetry::Buffers::Response> root) {
  fbb.Finish(root);
}

}  // namespace Buffers
}  // namespace Telemetry

#endif  // FLATBUFFERS_GENERATED_RESPONSE_TELEMETRY_BUFFERS_H_
