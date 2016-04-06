
#include "common.h"
#include "interpreter.h"


int main(int argc, char const *argv[]) {

  if (argc > 1) {
    cout << "ERROR: argumentos" << endl;
  }

  Reader r;
  Interpreter i(r);

  if (i.run()) {
    cout << "ERROR: <línea inválida>" << endl;
    return 2;
  }
  return 0;

}
