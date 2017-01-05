#include <zmq.hpp>

#include <zmq_helpers.h>
#include <json.hpp>

#include <iostream>
#include <thread>
#include <chrono>

class server {
 private:
  zmq::socket_t *socket;
  zmq::context_t context;
  const std::string addr;

 public:
  /**
   * Construct a server object with an address.
   *
   * @param addr
   */
  server(const std::string &addr) :
      context(zmq::context_t(1)),
      socket(nullptr),
      addr(addr) {}

  bool connect() {
    try {

      this->socket = new zmq::socket_t(this->context, ZMQ_REQ);
      this->socket->connect(this->addr);

    } catch (zmq::error_t &error) {
      std::cerr << error.what() << "\n";
      return false;
    }

    return true;
  }

  bool communicate(const std::string &message_str, std::string &reply) {
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
  // TODO: Make these configurable via command line arguments.
  std::vector<std::string> addresses = {
      "tcp://localhost:5555"
  };

  std::thread request_thread([&addresses]() {
    std::vector<zmq::socket_t> sockets;
    sockets.reserve(addresses.size());

    zmq::context_t context(1);

    // First, create the sockets.
    for ( auto const & address : addresses ) {
      sockets.push_back(zmq::socket_t(context, ZMQ_PUSH));

      try {
        sockets.back().connect(address);
      } catch ( zmq::error_t & exception ) {
        std::cerr << "Failed to connect to " << address << ", '" << exception.what() << "'\n";
      }
    }

    std::string message_str;
    zmq::message_t message;

    while (true) {
      std::cout << "Press Enter: ";

      getchar();

      message_str = "Please send me some data!";

      for ( auto & socket : sockets ) {
        message.rebuild(message_str.length());
        memcpy(message.data(), message_str.c_str(), message_str.length());

        // TODO: Research whether or not this blocks.
        socket.send(message);
      }
    }
  });

  std::thread aggregate_thread([]() {
    zmq::context_t context(1);
    zmq::socket_t receiver(context, ZMQ_PULL);

    receiver.connect("tcp://localhost:5556");

    zmq::message_t reply;
    std::string reply_str;

    while (true) {
      receiver.recv(&reply);

      zmq_extract_message(reply, reply_str);

      nlohmann::json j;
      try {
        j = nlohmann::json::parse(reply_str);
      } catch (std::invalid_argument & exception) {
        std::cout << "Invalid json encountered\n";
        continue;
      }

      std::cout << j.dump(4);
    }
  });

  request_thread.join();
  aggregate_thread.join();
}