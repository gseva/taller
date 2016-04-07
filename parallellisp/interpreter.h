#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include "common.h"
#include "expressions.h"
#include "parser.h"
#include "factories.h"


class Reader {

public:
  string nextLine() {
    return "(print (sum 2 3))";
  }

};


class Interpreter {

private:
  Reader reader_;

public:
  Interpreter(const Reader r) : reader_(r) {
  }

  int run() {
    Parser p;
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
