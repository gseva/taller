

#include <exception>
#include <string>
#include <sstream>
#include <stdexcept>
#include <sys/types.h>
#include <netdb.h>

#include "common_socket.h"


int Socket::connect(const std::string& hostname, const std::string& port) {
  struct addrinfo hints, *servinfo, *p;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  getaddrinfo(hostname.c_str(), port.c_str(), &hints, &servinfo);

  for (p = servinfo; p != NULL; p = p->ai_next) {
    if (::connect(fd_, p->ai_addr, p->ai_addrlen) != -1) {
      break;
    }
  }

  freeaddrinfo(servinfo);
  return 0;
}

int Socket::accept() {
  struct sockaddr_in cli_addr;
  socklen_t cli_len = sizeof(cli_addr);
  int newfd = ::accept(fd_, (struct sockaddr *) &cli_addr, &cli_len);

  return newfd;
}


int Socket::bind(const std::string& port) {
  std::stringstream ss(port);
  int p;
  ss >> p;
  struct sockaddr_in serv_addr;
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(p);

  return ::bind(fd_, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
}

int Socket::listen() {
  return ::listen(fd_, NUM_CLIENTS);
}


int Socket::bindAndListen(const std::string& port) {
  if (0 < bind(port)) {
    throw std::runtime_error("No pude bindear el puerto " + port);
  }
  return listen();
}


int Socket::write(const std::string& s) {
  int length = s.length();
  const char* data = s.c_str();
  while (length > 0)
  {
    int i = send(fd_, data, length, SOCKET_FLAGS);
    if (i < 1) return i;
    data += i;
    length -= i;
  }
  return 0;
}

std::string Socket::readLine() {
  std::string output;
  char c = '\0';

  while (c != '\n') {
    int i = recv(fd_, &c, 1, SOCKET_FLAGS);
    if (i < 1) throw std::runtime_error("No pude leer caracter!");
    output += c;
  }
  return output;
}

