#include <zmq.hpp>
#include <zmq_helpers.h>

#include <fstream>
#include <thread>

bool parse_config(const char *config_path, std::vector<std::string> & addresses)
{
  nlohmann::json config_json;
  std::ifstream json_file(config_path);

  if ( !json_file.good() ) {
    std::cerr << "Failed to open file " << config_path << " for reading\n";
    return false;
  }

  try { json_file >> config_json; }
  catch ( std::invalid_argument & e ) {
    std::cerr << "Failed to read json from " << config_path << "\n";
    return false;
  }

  // Check for nodes

  return true;
}

int main(int argc, char **argv) {
  if ( argc == 1 ) {
    std::cout << argv[0] << " path/to/config\n";

    return EXIT_FAILURE;
  }

  std::vector<std::string> addresses;
  parse_config(argv[1], addresses);

  return 0;
  std::thread request_thread([&addresses]() {
    std::vector<zmq::socket_t> sockets;
    sockets.reserve(addresses.size());

    zmq::context_t context(1);

    // setupSockets(addresses, sockets);
    // First, create the sockets.
    for ( auto const & address : addresses ) {
      sockets.push_back(zmq::socket_t(context, ZMQ_PUSH));

      try {
        sockets.back().connect(address);
      } catch ( zmq::error_t & exception ) {
        std::cerr << "Failed to connect to '" << address << "', '" << exception.what() << "'\n";
      }
    }

    // Request loop
    std::string message_str;
    zmq::message_t message;

    while (true) {
      std::cout << "Press Enter: ";

      getchar();

      message_str = "Please send me some data!";

      for ( auto & socket : sockets ) {
        zmq_pack_message(message, message_str);
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

    nlohmann::json reply_json;

    while (true) {
      receiver.recv(&reply);

      if ( !zmq_extract_json(reply, reply_json) ) {
        std::cerr << "Failed to parse JSON\n";
        continue;
      }

      std::cout << reply_json.dump(4);
    }
  });

  request_thread.join();
  aggregate_thread.join();
}