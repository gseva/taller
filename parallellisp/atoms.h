#ifndef __LISP_ATOMS_H__
#define __LISP_ATOMS_H__

#include "common.h"


class Atom {

public:

  virtual string getValue() = 0;

  virtual ~Atom() {}

};

class StringAtom : public Atom {

string value_;

public:

  void setValue(string s) {
    value_ = s;
  }

  virtual string getValue() {
    return value_;
  }

};


class NilAtom : public Atom {

public:

  void setValue(string s) {
  }

  virtual string getValue() {
    return "nil";
  }

};


class ListAtom : public Atom {

vector<Atom*> values;

public:

  void setValue(string s) {
  }

  bool isEmpty() {
    return values.size();
  }

  void addValue(Atom* value) {
    values.push_back(value);
  }

  virtual string getValue() {
    stringstream ss;
    ss << "(";
    for (int i = 0; i < values.size(); ++i) {
      ss << (values[i])->getValue();
      if (i != values.size() - 1) ss << " ";
    }
    ss << ")";

    return ss.str();
  }

  vector<Atom*>& getValues() {
    return values;
  }

};


#endif
