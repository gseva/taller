
#include <iostream>

#include "server.h"

int main(int argc, char const *argv[]) try {
  Server s(argv[1]);
  s.run();
  return 0;
} catch (const std::exception& e) {
  std::cout << e.what() << std::endl;
}
