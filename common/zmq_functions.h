#ifndef ZMQ_FUNCTIONS_INCLUDED
#define ZMQ_FUNCTIONS_INCLUDED

#include <cstdlib>
#include <string>

namespace zmq {
  class message_t;
}

namespace ZMQFunctions
{

/**
 * Extract a heap-allocated block from a zmq message.
 *
 * @param message
 * @data  pointer to a void pointer.
 * @len   Length of the message.
 */
bool extract(const zmq::message_t &message, void ** data, std::size_t & len);

/**
 * Extract a null terminated string from a zmq message.
 *
 * @param message
 * @param buffer
 */
bool extract_string(const zmq::message_t &message, std::string &buffer);

/**
 * Build out a zmq::message from a string.
 *
 * @param message
 * @param data
 */
bool pack(zmq::message_t &message, void *data, std::size_t len);

}

#endif
