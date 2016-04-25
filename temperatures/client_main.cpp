
#include <iostream>

#include "client.h"

int main(int argc, char const *argv[]) try {
  Client c(argv[1], argv[2]);
  c.run();
  return 0;
} catch (const std::exception& e) {
  std::cout << e.what() << std::endl;
}
