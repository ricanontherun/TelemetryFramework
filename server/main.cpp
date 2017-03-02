#include <iostream>
#include <iomanip>
#include <unistd.h>

#include <server.h>

int main(int argc, char **argv) {
  Networking::Options options;

  options.port = 5555;

  Networking::Server server;

  // Initialize the server.
  server.Init(options);

  // Start accepting client requests.
  server.Accept();

  return 0;
}

