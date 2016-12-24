#include "../vendor/zmq.hpp"

#include <zmq_helpers.h>

#include <iostream>
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
  // Set up the ventilator thread. This thread is responsible for periodically requesting
  // data from each of the monitored nodes.

  std::thread ventilator([]() {
    std::cout << "Ventilator: Starting thread.\n";

    zmq::context_t context(1);
    zmq::socket_t vent(context, ZMQ_PUSH);
    vent.connect("tcp://localhost:5555");

    std::cout << "Ventilator: Connected to localhost:5555\n";

    zmq::message_t message(5);
    memcpy(message.data(), "hello", 5);

    while (true) {
      std::cout << "Ventilator: Sending out requests...\n";

      vent.send(message);

      std::this_thread::sleep_for(std::chrono::seconds(2));
    }
  });

  std::thread sink([]() {
    std::cout << "Sink: Starting thread.\n";

    zmq::context_t context(1);
    zmq::socket_t receiver(context, ZMQ_PULL);
    receiver.connect("tcp://localhost:5556");

    std::cout << "Sink: Connected to localhost:5556\n";

    zmq::message_t reply;

    std::cout << "Sink: Waiting for messages from servers...\n";

    while (true) {
      receiver.recv(&reply);

      std::cout << "We received a message from a server!\n";
    }
  });

  ventilator.join();
  sink.join();
}