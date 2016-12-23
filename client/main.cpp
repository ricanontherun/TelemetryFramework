#include "../vendor/zmq.hpp"

#include <iostream>
#include <cstdio>
#include <cstdlib>

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

  server server("tcp://localhost:5555");
  bool connect = server.connect();

  std::string reply;
  server.communicate("hey man", reply);

  return 0;
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