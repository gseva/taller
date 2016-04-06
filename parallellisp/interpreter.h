#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include "common.h"
#include "parser.h"
#include "expressionFactory.h"


class Reader {

public:
  string nextLine() {
    return "(print 123)";
  }

};


class Interpreter {

private:
  Reader reader_;
  ExpressionFactory expFactory_;

public:
  Interpreter(const Reader r) : reader_(r) {
  }

  int run() {
    Parser p(expFactory_);
    Expression* e = p.parse(reader_.nextLine());
    if (e == NULL) return 1;
    Context c;
    e->eval(c);
    return 0;
  }

};


#endif
