#ifndef __COMMON_SOCKET_H__
#define __COMMON_SOCKET_H__

#include <string>
#include <sys/socket.h>

#define NUM_CLIENTS 5

#ifndef MSG_NOSIGNAL
#define SOCKET_FLAGS 0
#else
#define SOCKET_FLAGS MSG_NOSIGNAL
#endif


class Socket {
  int fd_;

public:
  Socket() {
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
  }
  explicit Socket(int fd) : fd_(fd) {
  }

  int connect(const std::string& hostname, const std::string& port);

  int write(const std::string& s);
  std::string readLine();

  int bind(const std::string& port);
  int accept();
  int listen();
  int bindAndListen(const std::string& port);

  void setNonBlocking();

  int close();

  ~Socket();
};

#endif
