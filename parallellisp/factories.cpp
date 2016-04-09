
#include <map>
#include <string>
#include <vector>

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


ExpressionFactory& Context::getExpressionFactory() {
  return expressionFactory_;
}

AtomFactory& Context::getAtomFactory() {
  return atomFactory_;
}

void Context::setAtom(std::string key, Atom* value) {
  atoms_[key] = value;
}

Atom* Context::getAtom(const std::string& key) {
  std::map<std::string,Atom*>::iterator it = atoms_.find(key);
  if (it == atoms_.end()) return NULL;

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
