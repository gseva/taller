#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include "expressions.h"
#include "parser.h"

#include <string>

class Reader {
public:
  virtual std::string nextLine();
};


class Interpreter {
  Reader reader_;

public:
  explicit Interpreter(const Reader r);

  int run();
};


#endif
