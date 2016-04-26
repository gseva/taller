
#include <iostream>
#include <sstream>
#include <string>

#include "client.h"


void Client::run() {
  sock_.connect(host_, port_);
  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.find_first_not_of(' ') == std::string::npos) {
      continue;
    }
    std::string res = map_(line);
    sock_.write(res + "\n");
  }
  sock_.write("End\n");
}


const std::string Mapper::operator() (const std::string& line) {
  std::stringstream ss(line);
  std::string city, temperature, day;

  ss >> city;
  ss >> temperature;
  ss >> day;

  return day + " " + city + " " + temperature;
}
