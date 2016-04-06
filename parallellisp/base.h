#ifndef __LISP_BASE_H__
#define __LISP_BASE_H__

#include "common.h"


class Atom;
class Context;
class Argument;
class Expression;



class Atom {

string value_;

public:
  Atom(string value) : value_(value) {
  }

  string getValue() {
    return value_;
  }

};


class Context {

};


class Argument {
  Atom a_;
  Expression* e_;
  bool isAtom_;

public:
  Argument(Atom a): a_(a), isAtom_(true) {

  }
  Argument(Expression* e): a_(NULL), e_(e), isAtom_(false){

  }

  Atom getAtom() {
    return a_;
  }

  Expression* getExpression() {
    return e_;
  }

  bool isAtom() {
    return isAtom_;
  }

};

class Expression {

deque<Argument> args_;

public:

  void addArgument(Argument a) {
    args_.push_back(a);
  }

  Atom getNextAgumentValue(Context &c) {
    Argument a = args_.front();
    args_.pop_front();

    if (a.isAtom()) {
      return a.getAtom();
    } else {
      Expression* e = a.getExpression();
      return e->eval(c);
    }

  }

  virtual Atom eval(Context &c) = 0;

  virtual ~Expression() {}

};



#endif
