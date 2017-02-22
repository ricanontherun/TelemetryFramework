#ifndef CLIENT_PIPE_H
#define CLIENT_PIPE_H

#include <networking/networking.h>

class ClientPipe : public Networking::AbstractServer
{
  public:
    ClientPipe() : Networking::AbstractServer(ZMQ_PULL, ZMQ_PUSH) {}
    void Init() {
    }
};

#endif
