#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <string>
#include "common_socket.h"


class Mapper {
public:
  const std::string operator() (const std::string& line);
};


class Client {
Socket sock_;
Mapper map_;
const std::string host_, port_;

public:
  Client(const std::string& host, const std::string& port) :
         host_(host), port_(port) {
  }

  void run();
};


#endif
