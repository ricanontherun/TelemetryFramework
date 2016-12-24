#include "../vendor/zmq.hpp"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <chrono>

class server
{
 private:
  zmq::socket_t * socket;
  zmq::context_t context;
  const std::string addr;

 public:
  /**
   * Construct a server object with an address.
   *
   * @param addr
   */
  server(const std::string & addr) :
      context(zmq::context_t(1)),
      socket(nullptr),
      addr(addr) {}

  bool connect() {
    try {

      this->socket = new zmq::socket_t(this->context, ZMQ_REQ);
      this->socket->connect(this->addr);

    } catch (zmq::error_t & error) {
      std::cerr << error.what() << "\n";
      return false;
    }

    return true;
  }

  bool communicate(const std::string & message_str, std::string & reply)
  {
    // this->send(this->make_message(message_str));

    // this->receive(reply);
    // return this->send(message_str)->receive(reply);
    std::size_t message_length = message_str.length();

    zmq::message_t message(message_length);

    memcpy(message.data(), message_str.c_str(), message_length);

    std::cout << (char *) message.data();
    // Construct a message.
    // Send it, block and wait for a reply.
    // Write it to reply.
  }

  ~server() {
    delete this->socket;
  }
};

int main() {
  // We need two threads. One which periodically requests data from each of the servers.
  // Another, the sink, which aggregates replies from the respective servers into a master JSON object.

  // First thread, which periodically requests some data.
  std::thread ventilator([]() {
    zmq::context_t context(1);
    zmq::socket_t vent(context, ZMQ_PUSH);

    // WHY DO WE BIND. We are sending data, it's not a server socket.
    vent.connect("tcp://localhost:5555");

    std::cout << "Ventilator setup.\n";

    zmq::message_t message(5);
    memcpy(message.data(), "hello", 5);

    while (true) {
      std::cout << "Ventilator: Sending out requests...\n";
      vent.send(message);
      std::this_thread::sleep_for(std::chrono::seconds(15));
    }
  });

  std::thread sink([]() {
    zmq::context_t context(1);

    // ZMQ_PULL is used for downstream nodes to pull data.
    zmq::socket_t receiver(context, ZMQ_PULL);

    // This socket listens on a port, thus we must bind and not connect().
    receiver.connect("tcp://localhost:5556");

    zmq::message_t reply;

    std::cout << "Sink initialized, waiting for messages from servers...\n";

    while (true) {
      receiver.recv(&reply);
      std::cout << "We received a message from a server!\n";
    }
  });

  ventilator.join();
  sink.join();
//
//  zmq::message_t message(5);
//  memcpy(message.data(), "hello", 5);
//
//  while (true) {
//    std::cout << "Press enter to send the next request\n";
//    getchar();
//
//    std::cout << "Sending '" << (char *) message.data() << "' to server\n";
//
//    sender.send(message);
//
//    // Wait for the reply
//    std::cout << "Waiting for the json reply...\n";
//    zmq::message_t reply;
//    sender.recv(&reply);
//    std::cout << "Received something back from server!\n";
//  }
}