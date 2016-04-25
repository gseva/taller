
#include <iostream>

#include "server.h"

void Server::run() {
  sock_.bindAndListen(port_);
  int fd = sock_.accept();
  Socket otroSock(fd);
  std::cout << otroSock.readLine() << std::endl;
}
