#ifndef NETWORKING_SERVER_H
#define NETWORKING_SERVER_H

#include <zmq.hpp>
#include <zmq_functions.h>
#include <telemetry.h>
#include <vector>
#include <thread>

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
    Server(int port);

    bool Init();

    void WorkerThread();

    bool HandleRequest(const Request & request, Reply & reply);

    void Accept();

  private:
    int port;

    zmq::context_t context;

    zmq::socket_t client_socket;
    zmq::socket_t worker_socket;

    std::size_t num_threads;
    std::vector<std::thread> workers_threads;

    Telemetry::Unit unit;
};

}

#endif
