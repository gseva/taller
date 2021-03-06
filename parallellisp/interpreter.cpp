
#include <string>
#include <iostream>

#include "interpreter.h"


std::string Reader::nextLine() {
  std::string buff;
  getline(std::cin, buff);

  // Remuevo trailing spaces
  size_t endpos = buff.find_last_not_of(" \t");
  if (std::string::npos != endpos) {
      buff = buff.substr(0, endpos + 1);
  }
  return buff;
}


Interpreter::Interpreter(const Reader r) : reader_(r) {
}

int Interpreter::run() {
  Mutex m;
  Context globalContext(m);
  Parser p(globalContext);

  std::string s = reader_.nextLine();
  while (s.size()) {
    Expression* e = p.parse(s);
    if (e == NULL) {
      std::cout << "ERROR: " << s << std::endl;
      return 1;
    }

    if (p.getParsingContext() == Sync) {
      e->eval(globalContext);
    } else {
      globalContext.runInThread(e);
    }
    // Context c;
    s = reader_.nextLine();
  }
  return 0;
}

