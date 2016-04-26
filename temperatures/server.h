#ifndef __SERVER_H__
#define __SERVER_H__

#include <map>
#include <string>
#include <vector>
#include "common_socket.h"
#include "common_thread.h"

typedef std::multimap<int, std::string> MapperResult;

class Runner;

class Reducer : public Thread {

std::string result_;
std::vector<std::string> values_;

public:
  Reducer(std::vector<std::string> values);

  void run();

  std::string getResult();

};

typedef std::map<int, Reducer*> ReducersMap;

class Accepter : public Thread {

Mutex m_;
Socket sock_;
bool stop_;
std::vector<Runner*> runners_;
MapperResult& result_;

void joinThreads_();

public:
  Accepter(const std::string& port, std::multimap<int, std::string>& r);
  void run();
  void stop();
  void insert(int key, const std::string& value);
  ~Accepter();
};

class Runner : public Thread {
Socket sock_;
Accepter* acc_;

public:
  Runner(int fd, Accepter* acc);
  void addLine(const std::string& line);
  void run();
};

class Server {

  const std::string port_;
  MapperResult mapResults_;
  ReducersMap reducers_;

  void accept_(Accepter& a);
  void spawnReducers_();
  void joinReducers_();
  void printResult_();

public:
  Server(const std::string& port) : port_(port) {
  }

  void run();

  ~Server();

};

#endif
