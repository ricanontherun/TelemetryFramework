#include "zmq_functions.h"

#include <zmq.hpp>

namespace ZMQFunctions {

bool extract(const zmq::message_t &message, void ** data, std::size_t & len)
{
  len = message.size();

  *data = malloc(len);

  if (!*data) {
    return false;
  }

  memcpy(*data, message.data(), len);

  return true;
}

bool extract_string(const zmq::message_t &message, std::string &buffer) {
  void * buf;
  std::size_t len = 0;

  if ( !extract(message, &buf, len) ) {
    return false;
  }

  buffer.assign((char *) buf, len);

  free(buf);

  return true;
}

bool pack(zmq::message_t &message, void *data, std::size_t len) {
  try {
    message.rebuild(len);
  } catch (zmq::error_t &e) {
    return false;
  }

  memcpy(message.data(), data, len);

  return true;
}

}
