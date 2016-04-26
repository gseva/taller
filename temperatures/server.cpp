
#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <sstream>
#include <vector>

#include "server.h"


using namespace std;



Reducer::Reducer(vector<string> values) : result_(""), values_(values) {
}

void Reducer::run() {
  int accTemperature = numeric_limits<int>::min();;
  vector<string> accCities;
  vector<string>::iterator it = values_.begin();
  for (; it != values_.end(); ++it) {
    string city;
    int temperature;
    istringstream ss(*it);

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
  ostringstream cityStream;
  copy(accCities.begin(), accCities.end(), ostream_iterator<string>(cityStream, "/"));
  string resultCities = cityStream.str();
  resultCities.erase(resultCities.length() - 1);

  ostringstream oss;
  oss << resultCities << " (" << accTemperature << ")";
  result_ = oss.str();
}

std::string Reducer::getResult() {
  return result_;
}


Runner::Runner(int fd, Accepter* acc) : sock_(Socket(fd)), acc_(acc) {
}

void Runner::addLine(const string& line) {
  int key;
  int pos = line.find_first_of(' ');
  istringstream iss(line.substr(0, pos));
  iss >> key;
  string value = line.substr(pos + 1);
  acc_->insert(key, value);
}

void Runner::run() {
  string result = sock_.readLine();
  while (result != "End\n") {
    addLine(result);
    result = sock_.readLine();
  }
}

void Accepter::joinThreads_() {
  vector<Runner*>::iterator i = runners_.begin();
  for(; i != runners_.end(); ++i) {
    (*i)->join();
  }
}

Accepter::Accepter(const string& port, multimap<int, string>& r)
                  : stop_(false), result_(r) {
  sock_.bindAndListen(port);
}

void Accepter::run() {
  while (!stop_) {
    int fd = sock_.accept();
    if (fd < 0) continue;
    Runner* r = new Runner(fd, this);
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
  result_.insert(pair<int, string>(key, value));
}

Accepter::~Accepter() {
  joinThreads_();
  for (size_t i = 0; i < runners_.size(); ++i) {
    delete runners_[i];
  }
}


void Server::accept_(Accepter& a) {
  a.start();
  string line("");
  while(line != "q") {
    getline(cin, line);
  }
  a.stop();
}

void Server::spawnReducers_() {
  pair<MapperResult::iterator, MapperResult::iterator> range;
  for(MapperResult::iterator it = mapResults_.begin(); it != mapResults_.end(); it = range.second) {
    range = mapResults_.equal_range(it->first);

    vector<string> values;

    MapperResult::iterator d = range.first;
    int key = (*d).first;
    for(; d != range.second; ++d) {
      values.push_back((*d).second);
    }

    Reducer* reducer = new Reducer(values);
    reducers_.insert(pair<int, Reducer*>(key, reducer));
    reducer->start();
  }
}

void Server::joinReducers_() {
  ReducersMap::iterator it = reducers_.begin();
  for (; it != reducers_.end(); ++it) {
    it->second->join();
  }
}

void Server::printResult_() {
  ReducersMap::iterator it = reducers_.begin();
  for (; it != reducers_.end(); ++it) {
    cout << it->first << ": " << it->second->getResult() << endl;
  }
}

void Server::run() {
  Accepter a(port_, mapResults_);
  accept_(a);
  a.join();
  spawnReducers_();
  joinReducers_();
  printResult_();
}

Server::~Server() {
  joinReducers_();
  ReducersMap::iterator it = reducers_.begin();
  for (; it != reducers_.end(); ++it) {
    delete it->second;
  }
}

