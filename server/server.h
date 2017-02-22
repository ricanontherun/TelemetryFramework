#ifndef NETWORKING_SERVER_H
#define NETWORKING_SERVER_H

#include <zmq.hpp>
#include <zmq_functions.h>
#include <telemetry.h>

namespace Networking
{

class Request
{
  friend class Server;

  private:
    zmq::message_t request;

  public:
    Request() {}
};

class Reply
{
  friend class Server;

  private:
    zmq::message_t reply;
  public:
    Reply() {};
};

class Server
{
  public:
    Server();

    bool Init();

    void WaitForClient(Request & request);

    bool HandleRequest(const Request & request, Reply & reply);

    bool SendReply(Reply & reply);
  private:
    zmq::context_t context;
    zmq::socket_t request_socket;
    zmq::socket_t reply_socket;


    Telemetry::Unit unit;
};

}

#endif
