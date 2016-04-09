#ifndef __EXPRESSION_FACTORY_H
#define __EXPRESSION_FACTORY_H

#include <map>
#include <string>
#include <vector>

#include "expressions.h"
#include "atoms.h"
#include "thread.h"


class Context;


template <class T>
class Factory {
  std::vector<T*> createdObjects_;

public:
  template <class U>
  U* createObject() {
    U* var = new U();
    createdObjects_.push_back((U*) var);
    return var;
  }

  virtual ~Factory() {
    for (size_t i = 0; i < createdObjects_.size(); ++i) {
      delete createdObjects_[i];
    }
  }
};


class ExpressionFactory : private Factory<Expression> {
public:
  PrintExpression* createPrint();
  SumExpression* createSum();
  DiffExpression* createDiff();
  MulExpression* createMul();
  DivExpression* createDiv();
  ListExpression* createList();
  CarExpression* createCar();
  CdrExpression* createCdr();
  AppendExpression* createAppend();
  IfExpression* createIf();
  SetqExpression* createSetq();
  SyncExpression* createSync();
};


class AtomFactory : private Factory<Atom> {
public:
  StringAtom* createString();
  NumericAtom* createNumeric();
  ListAtom* createList();
};


class ExpressionRunner : public Thread {
Context* c_;
Expression* e_;
public:
  ExpressionRunner();

  void setParameters(Context* c, Expression* e);
  virtual void run();
};


class ExpressionRunnerFactory : private Factory<Thread> {
public:
  ExpressionRunner* createRunner();
};


class Context {
ExpressionFactory expressionFactory_;
AtomFactory atomFactory_;
ExpressionRunnerFactory runnerFactory_;

std::map<std::string,Atom*> atoms_;
std::vector<ExpressionRunner*> threads_;

public:
  ExpressionFactory& getExpressionFactory();
  AtomFactory& getAtomFactory();

  void setAtom(std::string key, Atom* value);
  Atom* getAtom(const std::string& key);

  void runInThread(Expression* e);
  void joinThreads();

  ~Context();
};


#endif
