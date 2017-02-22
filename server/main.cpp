#include <iostream>
#include <iomanip>
#include <unistd.h>

#include <server.h>

int main() {
  Networking::Server server;

  server.Init();

  Networking::Request request;
  Networking::Reply reply;

  while (true) {
    server.WaitForClient(request);

    // Next we need to parse out the request command and the arguments and all that.
    server.HandleRequest(request, reply);

    server.SendReply(reply);
  }
}

