
#include <map>
#include <string>
#include <vector>
#include <iostream>

#include "factories.h"


PrintExpression* ExpressionFactory::createPrint() {
  return createObject<PrintExpression>();
}

SumExpression* ExpressionFactory::createSum() {
  return createObject<SumExpression>();
}

DiffExpression* ExpressionFactory::createDiff() {
  return createObject<DiffExpression>();
}

MulExpression* ExpressionFactory::createMul() {
  return createObject<MulExpression>();
}

DivExpression* ExpressionFactory::createDiv() {
  return createObject<DivExpression>();
}

EqualExpression* ExpressionFactory::createEqual() {
  return createObject<EqualExpression>();
}

LesserExpression* ExpressionFactory::createLesser() {
  return createObject<LesserExpression>();
}

GreaterExpression* ExpressionFactory::createGreater() {
  return createObject<GreaterExpression>();
}

ListExpression* ExpressionFactory::createList() {
  return createObject<ListExpression>();
}

CarExpression* ExpressionFactory::createCar() {
  return createObject<CarExpression>();
}

CdrExpression* ExpressionFactory::createCdr() {
  return createObject<CdrExpression>();
}

AppendExpression* ExpressionFactory::createAppend() {
  return createObject<AppendExpression>();
}

IfExpression* ExpressionFactory::createIf() {
  return createObject<IfExpression>();
}

SetqExpression* ExpressionFactory::createSetq() {
  return createObject<SetqExpression>();
}

SyncExpression* ExpressionFactory::createSync() {
  return createObject<SyncExpression>();
}

DefunExpression* ExpressionFactory::createDefun() {
  return createObject<DefunExpression>();
}


StringAtom* AtomFactory::createString() {
  return createObject<StringAtom>();
}

NumericAtom* AtomFactory::createNumeric() {
  return createObject<NumericAtom>();
}

ListAtom* AtomFactory::createList() {
  return createObject<ListAtom>();
}


ExpressionRunner::ExpressionRunner() : c_(NULL), e_(NULL) {
}

void ExpressionRunner::setParameters(Context* c, Expression* e) {
  c_ = c;
  e_ = e;
}

void ExpressionRunner::run() {
  e_->eval(*c_);
}


ExpressionRunner* ExpressionRunnerFactory::createRunner() {
  return createObject<ExpressionRunner>();
}


Context::Context(Mutex& m) : m_(m) {
}

Mutex& Context::getMutex() {
  return m_;
}

ExpressionFactory& Context::getExpressionFactory() {
  return expressionFactory_;
}

AtomFactory& Context::getAtomFactory() {
  return atomFactory_;
}

void Context::setAtom(std::string key, Atom* value) {
  Lock l(m_);
  atoms_[key] = value;
}

Atom* Context::getAtom(const std::string& key) {
  std::map<std::string,Atom*>::iterator it = atoms_.find(key);
  if (it == atoms_.end()) return NULL;

  return (*it).second;
}

void Context::setExpression(std::string key, Expression* value) {
  Lock l(m_);
  expressions_[key] = value;
}

Expression* Context::getExpression(const std::string& key) {
  std::map<std::string,Expression*>::iterator it = expressions_.find(key);
  if (it == expressions_.end()) return NULL;

  return (*it).second;
}

void Context::runInThread(Expression* e) {
  ExpressionRunner* er = runnerFactory_.createRunner();
  er->setParameters(this, e);
  threads_.push_back(er);
  er->start();
}

void Context::joinThreads() {
  std::vector<ExpressionRunner*>::iterator it = threads_.begin();
  for (; it != threads_.end();) {
    (*it)->join();
    it = threads_.erase(it);
  }
}

Context::~Context() {
  joinThreads();
}
