#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include "common.h"
#include "expressions.h"
#include "parser.h"


class Reader {

public:
  string nextLine() {
    return "(print (list 1 2 3 4))";
  }

};


class Interpreter {

private:
  Reader reader_;

public:
  Interpreter(const Reader r) : reader_(r) {
  }

  int run() {
    Context globalContext;
    Parser p(globalContext);
    string s = reader_.nextLine();
    Expression* e = p.parse(s);
    if (e == NULL) {
      cout << "ERROR: " << s << endl;
      return 1;
    }
    Context c;
    e->eval(c);
    return 0;
  }

};


#endif
