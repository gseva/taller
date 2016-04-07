#ifndef __LISP_ATOMS_H__
#define __LISP_ATOMS_H__

#include "common.h"

class Atom {

string value_;

public:
  Atom(string value) : value_(value) {
  }

  string getValue() {
    return value_;
  }

};


class NilAtom : public Atom {

public:
  NilAtom() : Atom("nil") {
  }

};

#endif
