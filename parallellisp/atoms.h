#ifndef __LISP_ATOMS_H__
#define __LISP_ATOMS_H__

class NilAtom : public Atom {

public:
  NilAtom() : Atom("nil") {
  }

};

#endif
