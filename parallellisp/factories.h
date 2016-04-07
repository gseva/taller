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
  U* createExpression() {
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
    return createExpression<PrintExpression>();
  }

  SumExpression* createSum() {
    return createExpression<SumExpression>();
  }

  DiffExpression* createDiff() {
    return createExpression<DiffExpression>();
  }

  MulExpression* createMul() {
    return createExpression<MulExpression>();
  }

  DivExpression* createDiv() {
    return createExpression<DivExpression>();
  }


};

#endif
