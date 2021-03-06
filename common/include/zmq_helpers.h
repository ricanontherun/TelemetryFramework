#ifndef ZMQ_HELPERS
#define ZMQ_HELPERS

#include "json.hpp"

/**
 * Extract a null terminated string from a zmq message.
 *
 * @param message
 * @param buffer
 */
bool zmq_extract_message(const zmq::message_t &message, std::string &buffer) {
  std::size_t size = message.size();

  char *buf = (char *) malloc(size);

  if (!buf) {
    return false;
  }

  memcpy((void *) buf, message.data(), size);

  buffer.assign(buf, size);

  free(buf);

  return true;
}

/**
 * Extract a json object from a zmq message.
 *
 * @param message
 * @param json
 * @return
 */
bool zmq_extract_json(const zmq::message_t &message, nlohmann::json &json) {
  std::string message_buffer;

  if (!zmq_extract_message(message, message_buffer)) {
    return false;
  }

  try {

    json = nlohmann::json::parse(message_buffer);

    return true;
  } catch (std::invalid_argument &e) {
    return false;
  }
}

/**
 * Build out a zmq::message from a string.
 *
 * @param message
 * @param data
 */
bool zmq_pack_message(zmq::message_t &message, std::string data) {
  std::size_t data_length = data.length();

  try {
    message.rebuild(data_length);
  } catch (zmq::error_t &e) {
    return false;
  }

  memcpy(message.data(), data.c_str(), data_length);

  return true;
}

bool zmq_pack_message(zmq::message_t & message, void *data, std::size_t len) {
  try {
    message.rebuild(len);
  } catch (zmq::error_t &e) {
    return false;
  }

  memcpy(message.data(), data, len);

  return true;
}

#endif
