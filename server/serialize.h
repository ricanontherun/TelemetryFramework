#ifndef SERIALIZATION_H
#define SERIALIZATION_H

namespace Telemetry{
  class Results;
}

namespace flatbuffers {
  class FlatBufferBuilder;
}

namespace Application
{
  /**
   * Serialize a Telemetry::Results object into a Flatbuffer response.
   *
   * @param results
   * @param builder
   */
  void SerializeResults(const Telemetry::Results & results, flatbuffers::FlatBufferBuilder & builder);
}

#endif
