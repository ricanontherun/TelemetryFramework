// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_USAGE_TELEMETRY_BUFFERS_H_
#define FLATBUFFERS_GENERATED_USAGE_TELEMETRY_BUFFERS_H_

#include "flatbuffers/flatbuffers.h"

namespace Telemetry {
namespace Buffers {

struct Usage;

MANUALLY_ALIGNED_STRUCT(4) Usage FLATBUFFERS_FINAL_CLASS {
 private:
  float actual_;
  float relative_;

 public:
  Usage() {
    memset(this, 0, sizeof(Usage));
  }
  Usage(const Usage &_o) {
    memcpy(this, &_o, sizeof(Usage));
  }
  Usage(float _actual, float _relative)
      : actual_(flatbuffers::EndianScalar(_actual)),
        relative_(flatbuffers::EndianScalar(_relative)) {
  }
  float actual() const {
    return flatbuffers::EndianScalar(actual_);
  }
  float relative() const {
    return flatbuffers::EndianScalar(relative_);
  }
};
STRUCT_END(Usage, 8);

}  // namespace Buffers
}  // namespace Telemetry

#endif  // FLATBUFFERS_GENERATED_USAGE_TELEMETRY_BUFFERS_H_
