#ifndef NETWORKING_CLIENT_H
#define NETWORKING_CLIENT_H

#include <zmq.hpp>

#include <string>
#include <vector>

namespace flatbuffers
{
class FlatBufferBuilder;
}

namespace Networking
{

class Client
{
  public:
    Client();

    void AddServer(const std::string & address);
    void Read(int resources);

  private:
    zmq::context_t context;
    std::vector<zmq::socket_t> servers;

    void BuildReadProcedure(flatbuffers::FlatBufferBuilder & builder, int resources);
};

}

#endif
