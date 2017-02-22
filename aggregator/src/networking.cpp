#include <networking/networking.h>

#include <zmq.hpp>

namespace Networking {

Message::Message() {}

AbstractServer::AbstractServer(int inbound_type, int outbound_type)
: context(1), inbound_socket(context, inbound_type), outbound_socket(context, outbound_type) {}

void AbstractServer::Receive(Message & inbound)
{
  this->inbound_socket.recv(&(inbound.message));
}

bool AbstractServer::Send(Message & outbound)
{
  this->outbound_socket.send(outbound.message);

  return true;
}

}
