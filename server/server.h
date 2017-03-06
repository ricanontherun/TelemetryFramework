#ifndef NETWORKING_SERVER_H
#define NETWORKING_SERVER_H

#include <zmq.hpp>
#include <zmq_functions.h>
#include <telemetry.h>
#include <request_generated.h>
#include <response_generated.h>
#include <filesystem_generated.h>
#include <usage_generated.h>

#include <vector>
#include <thread>

namespace Networking
{

struct Options
{
  int port;

  Options()
  : port(32456)
  {}
};

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

    bool Init(const Options & options);

    void WorkerThread();

    bool HandleRequest(const zmq::message_t & request, zmq::message_t & reply);

    void Accept();

  private:
    zmq::context_t context;

    // The socket clients connect to.
    zmq::socket_t client_socket;

    // The socket which queues the client request for processing
    // in the worker threads.
    zmq::socket_t worker_socket;

    std::size_t num_threads;
    std::vector<std::thread> workers_threads;

    bool ValidateRequestBuffer(
        std::uint8_t * buffer_pointer,
        const std::size_t & buffer_length
    ) const;

    void DoReadProcedure(
        const Telemetry::Buffers::ReadProcedure * procedure,
        zmq::message_t & reply
    )
    const;
};

}

#endif
