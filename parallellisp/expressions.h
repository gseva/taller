#ifndef __LIST_EXPRESSIONS_H__
#define __LIST_EXPRESSIONS_H__

#include "common.h"
#include "atoms.h"


class Expression;


class Context {

};


class Argument {
  Atom a_;
  Expression* e_;
  bool isAtom_;

public:
  Argument(Atom a): a_(a), isAtom_(true) {

  }
  Argument(Expression* e): a_(""), e_(e), isAtom_(false){

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

  void addArgument(Expression* e) {
    Argument arg(e);
    args_.push_back(arg);
  }

  void addArgument(Atom a) {
    Argument arg(a);
    args_.push_back(arg);
  }

  Atom getArgumentValue(Argument a, Context &c) {
    if (a.isAtom()) {
      return a.getAtom();
    } else {
      Expression* e = a.getExpression();
      return e->eval(c);
    }
  }

  deque<Argument>& getArguments() {
    return args_;
  }

  virtual Atom eval(Context &c) = 0;

  virtual ~Expression() {}

};


class PrintExpression : public Expression {

public:
  virtual Atom eval(Context &c) {

    deque<Argument> args_ = getArguments();

    deque<Argument>::iterator it = args_.begin();
    for (; it != args_.end(); ++it) {
      Atom a = getArgumentValue(*it, c);
      cout << a.getValue() << endl;
    }

    NilAtom nil;
    return nil;
  }

};


class SumExpression : public Expression {

public:

  virtual Atom eval(Context &c) {
    int value = 0;
    deque<Argument> args_ = getArguments();

    deque<Argument>::iterator it = args_.begin();
    for (; it != args_.end(); ++it) {
      Atom a = getArgumentValue(*it, c);
      value += stoi(a.getValue());
    }

    stringstream ss;
    ss << value;
    Atom result(ss.str());
    return result;
  }

};

#endif
