#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include "common.h"
#include "expressions.h"
#include "parser.h"


class Reader {

public:

  bool end() {
    return (bool) cin;
  }

  string nextLine() {
    string buff;
    getline(cin, buff);
    return buff;
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
    while (s.size()) {
      Expression* e = p.parse(s);
      if (e == NULL) {
        cout << "ERROR: " << s << endl;
        return 1;
      }
      Context c;
      e->eval(c);
      s = reader_.nextLine();
    }
    return 0;

  }

};


#endif
