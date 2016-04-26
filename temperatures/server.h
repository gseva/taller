#ifndef __SERVER_H__
#define __SERVER_H__

#include <map>
#include <string>
#include <vector>
#include "common_socket.h"
#include "common_thread.h"

typedef std::multimap<int, std::string> MapperResult;

class Connection;

class Reducer : public Thread {
std::string result_;
std::vector<std::string> values_;

public:
  explicit Reducer(std::vector<std::string> values);

  virtual void run();

  std::string getResult();
};

typedef std::map<int, Reducer*> ReducersMap;

class Accepter : public Thread {
Mutex m_;
Socket sock_;
bool stop_;
std::vector<Connection*> runners_;
MapperResult& result_;

void joinThreads_();

public:
  Accepter(const std::string& port, std::multimap<int, std::string>& r);
  virtual void run();
  void stop();
  void insert(int key, const std::string& value);
  ~Accepter();
};

class Connection : public Thread {
Socket sock_;
Accepter* acc_;

public:
  Connection(int fd, Accepter* acc);
  void addLine(const std::string& line);
  virtual void run();
};

class Server {
  const std::string port_;
  MapperResult mapResults_;
  ReducersMap reducers_;

  void accept_(Accepter& a);
  void spawnReducers_();
  void joinReducers_();

public:
  explicit Server(const std::string& port);

  void run();

  ~Server();
};

#endif
