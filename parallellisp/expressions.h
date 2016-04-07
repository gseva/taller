#ifndef __LIST_EXPRESSIONS_H__
#define __LIST_EXPRESSIONS_H__

#include "common.h"
#include "atoms.h"


class Context;
class Expression;


class Argument {
  Atom* a_;
  Expression* e_;
  bool isAtom_;

public:
  Argument(Atom* a): a_(a), isAtom_(true) {

  }
  Argument(Expression* e): e_(e), isAtom_(false){

  }

  Atom* getAtom() {
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

  void addArgument(Atom* a) {
    Argument arg(a);
    args_.push_back(arg);
  }

  Atom* getArgumentValue(Argument a, Context& c);

  deque<Argument>& getArguments() {
    return args_;
  }

  virtual Atom* eval(Context& c) = 0;

  virtual ~Expression() {}

};


class PrintExpression : public Expression {

public:
  virtual Atom* eval(Context& c);

};


class MathExpression : public Expression {

public:

  virtual int operation(int a, int v) = 0;

  virtual Atom* eval(Context& c);

};

class SumExpression : public MathExpression {

  virtual int operation(int a, int b) {
    return a + b;
  }

};

class DiffExpression : public MathExpression {

  virtual int operation(int a, int b) {
    return a - b;
  }

};

class MulExpression : public MathExpression {

  virtual int operation(int a, int b) {
    return a * b;
  }

};

class DivExpression : public MathExpression {

  virtual int operation(int a, int b) {
    return a / b;
  }

};


class ListExpression : public Expression {

public:
  virtual Atom* eval(Context& c);

};


#endif
