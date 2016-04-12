#ifndef __LIST_EXPRESSIONS_H__
#define __LIST_EXPRESSIONS_H__

#include <deque>
#include <vector>
#include <string>

#include "atoms.h"


class Context;
class Expression;


class Argument {
  Atom* a_;
  Expression* e_;
  bool isAtom_;

public:
  explicit Argument(Atom* a);
  explicit Argument(Expression* e);
  Argument();

  void setAtom(Atom* a);
  void setExpression(Expression* e);

  Atom* getAtom();
  Expression* getExpression();
  bool isAtom();
};


class Expression {
std::deque<Argument*> args_;

public:
  void addArgument(Expression* e);
  void addArgument(Atom* a);

  Atom* getArgumentValue(Argument* a, Context& c);
  std::deque<Argument*>& getArguments();

  ListAtom* createNil(Context &c);

  virtual Atom* eval(Context& c) = 0;
  virtual ~Expression();
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
  virtual int operation(int a, int b);
};

class DiffExpression : public MathExpression {
  virtual int operation(int a, int b);
};

class MulExpression : public MathExpression {
  virtual int operation(int a, int b);
};

class DivExpression : public MathExpression {
  virtual int operation(int a, int b);
};

class EqualExpression : public Expression {
public:
  virtual Atom* eval(Context& c);
  virtual bool compare(const std::string& a, const std::string& b);
};

class LesserExpression : public EqualExpression {
public:
  virtual bool compare(const std::string& a, const std::string& b);
};

class GreaterExpression : public EqualExpression {
public:
  virtual bool compare(const std::string& a, const std::string& b);
};

class ListExpression : public Expression {
public:
  virtual Atom* eval(Context& c);
};

class CarExpression : public Expression {
public:
  virtual Atom* extractAtom(std::vector<Atom*>& values, Context& c);
  virtual Atom* eval(Context& c);
};

class CdrExpression : public CarExpression {
public:
  virtual Atom* extractAtom(std::vector<Atom*>& values, Context& c);
};

class AppendExpression : public Expression {
public:
  virtual Atom* eval(Context& c);
};

class IfExpression : public Expression {
public:
  virtual Atom* eval(Context& c);
};

class SetqExpression : public Expression {
public:
  virtual Atom* eval(Context& c);
};

class SyncExpression : public Expression {
public:
  virtual Atom* eval(Context& c);
};

class DefunExpression : public Expression {
std::string expression_;

public:
  virtual Atom* eval(Context& c);

  void setExpressionString(std::string s);
  std::string getExpressionString(std::string parameters);
};

#endif
