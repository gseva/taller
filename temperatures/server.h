#ifndef __SERVER_H__
#define __SERVER_H__

#include <string>
#include "common_socket.h"

class Server {

  Socket sock_;
  const std::string port_;

public:
  Server(const std::string& port) : port_(port) {
  }

  void run();

};

#endif
