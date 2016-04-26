
#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <utility>

#include "server.h"


using std::vector;
using std::string;


Reducer::Reducer(vector<string> values) : result_(""), values_(values) {
}

void Reducer::run() {
  int accTemperature = std::numeric_limits<int>::min();;
  vector<string> accCities;
  vector<string>::iterator it = values_.begin();
  for (; it != values_.end(); ++it) {
    string city;
    int temperature;
    std::istringstream ss(*it);

    ss >> city;
    ss >> temperature;
    if (temperature > accTemperature && city.size()) {
      accTemperature = temperature;
      vector<string> cities;
      cities.push_back(city);
      accCities.assign(cities.begin(), cities.end());
    } else if (temperature == accTemperature) {
      accCities.push_back(city);
    }
  }

  sort(accCities.begin(), accCities.end());
  std::ostringstream cityStream;
  copy(accCities.begin(), accCities.end(),
       std::ostream_iterator<string>(cityStream, "/"));
  string resultCities = cityStream.str();
  resultCities.erase(resultCities.length() - 1);

  std::ostringstream oss;
  oss << resultCities << " (" << accTemperature << ")";
  result_ = oss.str();
}

std::string Reducer::getResult() {
  return result_;
}


Connection::Connection(int fd, Accepter* acc) : sock_(Socket(fd)), acc_(acc) {
}

void Connection::addLine(const string& line) {
  int key;
  int pos = line.find_first_of(' ');
  std::istringstream iss(line.substr(0, pos));
  iss >> key;
  string value = line.substr(pos + 1);
  acc_->insert(key, value);
}

void Connection::run() {
  string result = sock_.readLine();
  while (result != "End\n") {
    addLine(result);
    result = sock_.readLine();
  }
}

void Accepter::joinThreads_() {
  vector<Connection*>::iterator it = runners_.begin();
  for (; it != runners_.end();) {
    (*it)->join();
    delete *it;
    it = runners_.erase(it);
  }
}

Accepter::Accepter(const string& port, MapperResult& r)
                  : stop_(false), result_(r) {
  sock_.bindAndListen(port);
}

void Accepter::run() {
  while (!stop_) {
    int fd = sock_.accept();
    if (fd < 0) continue;
    Connection* r = new Connection(fd, this);
    r->start();
    runners_.push_back(r);
  }
}

void Accepter::stop() {
  stop_ = true;
  sock_.close();
  joinThreads_();
}

void Accepter::insert(int key, const string& value) {
  Lock l(m_);
  result_.insert(std::pair<int, string>(key, value));
}

Accepter::~Accepter() {
  joinThreads_();
}


Server::Server(const std::string& port) : port_(port) {
}

void Server::accept_(Accepter& a) {
  a.start();
  string line("");
  while (line != "q") {
    getline(std::cin, line);
  }
  a.stop();
}

void Server::spawnReducers_() {
  std::pair<MapperResult::iterator, MapperResult::iterator> range;
  MapperResult::iterator it = mapResults_.begin();
  for (; it != mapResults_.end(); it = range.second) {
    range = mapResults_.equal_range(it->first);

    vector<string> values;

    MapperResult::iterator d = range.first;
    int key = (*d).first;
    for (; d != range.second; ++d) {
      values.push_back((*d).second);
    }

    Reducer* reducer = new Reducer(values);
    reducers_.insert(std::pair<int, Reducer*>(key, reducer));
    reducer->start();
  }
}

void Server::joinReducers_() {
  ReducersMap::iterator it = reducers_.begin();
  for (; it != reducers_.end();) {
    it->second->join();
    std::cout << it->first << ": " << it->second->getResult() << std::endl;
    delete it->second;
    reducers_.erase(it++);
  }
}

void Server::run() {
  Accepter a(port_, mapResults_);
  accept_(a);
  a.join();
  spawnReducers_();
  joinReducers_();
}

Server::~Server() {
  joinReducers_();
}
