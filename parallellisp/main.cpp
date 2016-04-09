
#include <iostream>

#include "interpreter.h"


int main(int argc, char const *argv[]) {
  if (argc > 1) {
    std::cout << "ERROR: argumentos" << std::endl;
    return 1;
  }

  Reader r;
  Interpreter i(r);

  if (i.run()) return 2;

  return 0;
}
