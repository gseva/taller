#ifndef __EXPRESSION_FACTORY_H
#define __EXPRESSION_FACTORY_H

#include "common.h"
#include "expressions.h"
#include "atoms.h"


template <class T>
class Factory {

  vector<T*> createdObjects_;

public:

  template <class U>
  U* createObject() {
    U* var = new U();
    createdObjects_.push_back((U*) var);
    return var;
  }

  virtual ~Factory() {
    for (int i = 0; i < createdObjects_.size(); ++i) {
      delete createdObjects_[i];
    }
  }

};


class ExpressionFactory : private Factory<Expression> {

public:

  PrintExpression* createPrint() {
    return createObject<PrintExpression>();
  }

  SumExpression* createSum() {
    return createObject<SumExpression>();
  }

  DiffExpression* createDiff() {
    return createObject<DiffExpression>();
  }

  MulExpression* createMul() {
    return createObject<MulExpression>();
  }

  DivExpression* createDiv() {
    return createObject<DivExpression>();
  }

  ListExpression* createList() {
    return createObject<ListExpression>();
  }

};


class AtomFactory : private Factory<Atom> {

public:

  StringAtom* createString() {
    return createObject<StringAtom>();
  }

  NilAtom* createNil() {
    return createObject<NilAtom>();
  }

  ListAtom* createList() {
    return createObject<ListAtom>();
  }


};


class Context {

ExpressionFactory expressionFactory_;
AtomFactory atomFactory_;

public:

  ExpressionFactory& getExpressionFactory() {
    return expressionFactory_;
  }

  AtomFactory& getAtomFactory() {
    return atomFactory_;
  }

};


#endif
