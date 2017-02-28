#include <iostream>
#include <iomanip>
#include <unistd.h>

#include <server.h>

int main() {
  Networking::Server server(5555);

  // Initialize the server.
  server.Init();

  // Start accepting client requests.
  server.Accept();

  return 0;
}

