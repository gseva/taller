#ifndef __LIST_EXPRESSIONS_H__
#define __LIST_EXPRESSIONS_H__

#include "common.h"
#include "base.h"
#include "atoms.h"


class Print : public Expression {

public:
  virtual Atom eval(Context &c) {
    Atom a = this->getNextAgumentValue(c);
    cout << a.getValue() << endl;
    NilAtom n;
    return n;
  }

};

#endif
