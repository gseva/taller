
#include <map>
#include <string>
#include <vector>
#include <iostream>

#include "factories.h"


ExpressionRunner::ExpressionRunner() : c_(NULL), e_(NULL) {
}

void ExpressionRunner::setParameters(Context* c, Expression* e) {
  c_ = c;
  e_ = e;
}

void ExpressionRunner::run() {
  e_->eval(*c_);
}


Context::Context(Mutex& m) : m_(m) {
}

Mutex& Context::getMutex() {
  return m_;
}

Factory<Expression>& Context::getExpressionFactory() {
  return expressionFactory_;
}

Factory<Atom>& Context::getAtomFactory() {
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
  ExpressionRunner* er = runnerFactory_.createObject<ExpressionRunner>();
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
