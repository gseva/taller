#ifndef __EXPRESSION_FACTORY_H
#define __EXPRESSION_FACTORY_H

#include "common.h"
#include "base.h"
#include "expressions.h"

class ExpressionFactory {

private:
  vector<Expression*> createdExpressions_;

public:
  Print* createPrint() {
    Print* p = new Print();
    createdExpressions_.push_back(p);
    return p;
  }

  ~ExpressionFactory() {
    for (int i = 0; i < createdExpressions_.size(); ++i) {
      delete createdExpressions_[i];
    }
  }

};

#endif
