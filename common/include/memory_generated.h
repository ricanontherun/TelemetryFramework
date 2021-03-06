// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_MEMORY_TELEMETRY_BUFFERS_H_
#define FLATBUFFERS_GENERATED_MEMORY_TELEMETRY_BUFFERS_H_

#include "flatbuffers/flatbuffers.h"

#include "usage_generated.h"

namespace Telemetry {
namespace Buffers {

struct Memory;

struct Memory FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_TOTAL = 4,
    VT_USED = 6,
    VT_FREE = 8
  };
  uint64_t total() const {
    return GetField<uint64_t>(VT_TOTAL, 0);
  }
  const Telemetry::Buffers::Usage *used() const {
    return GetStruct<const Telemetry::Buffers::Usage *>(VT_USED);
  }
  const Telemetry::Buffers::Usage *free() const {
    return GetStruct<const Telemetry::Buffers::Usage *>(VT_FREE);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint64_t>(verifier, VT_TOTAL) &&
           VerifyField<Telemetry::Buffers::Usage>(verifier, VT_USED) &&
           VerifyField<Telemetry::Buffers::Usage>(verifier, VT_FREE) &&
           verifier.EndTable();
  }
};

struct MemoryBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_total(uint64_t total) {
    fbb_.AddElement<uint64_t>(Memory::VT_TOTAL, total, 0);
  }
  void add_used(const Telemetry::Buffers::Usage *used) {
    fbb_.AddStruct(Memory::VT_USED, used);
  }
  void add_free(const Telemetry::Buffers::Usage *free) {
    fbb_.AddStruct(Memory::VT_FREE, free);
  }
  MemoryBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  MemoryBuilder &operator=(const MemoryBuilder &);
  flatbuffers::Offset<Memory> Finish() {
    const auto end = fbb_.EndTable(start_, 3);
    auto o = flatbuffers::Offset<Memory>(end);
    return o;
  }
};

inline flatbuffers::Offset<Memory> CreateMemory(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint64_t total = 0,
    const Telemetry::Buffers::Usage *used = 0,
    const Telemetry::Buffers::Usage *free = 0) {
  MemoryBuilder builder_(_fbb);
  builder_.add_total(total);
  builder_.add_free(free);
  builder_.add_used(used);
  return builder_.Finish();
}

}  // namespace Buffers
}  // namespace Telemetry

#endif  // FLATBUFFERS_GENERATED_MEMORY_TELEMETRY_BUFFERS_H_
