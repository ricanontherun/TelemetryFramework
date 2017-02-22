#ifndef NETWORKING_H
#define NETWORKING_H

#include <zmq.hpp>

namespace Networking
{
  class Message
  {
    friend class AbstractServer;

    private:
      zmq::message_t message;
    public:
      Message();
  };

  class AbstractServer
  {
    public:
      AbstractServer(int inbound_type, int outbound_type);
      void Receive(Message & inbound);
      bool Send(Message & outbound);

      // This is where implementors can bind to internet sockets.
      virtual void Init() = 0;

    protected:
      zmq::context_t context;
      zmq::socket_t outbound_socket;
      zmq::socket_t inbound_socket;
  };
}

#endif
