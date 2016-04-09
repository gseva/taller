#ifndef __LISP_ATOMS_H__
#define __LISP_ATOMS_H__

#include <string>
#include <vector>


class Atom {
public:
  virtual bool isTrue() { return true; }

  virtual std::string getValue() = 0;

  virtual ~Atom() {}
};

class StringAtom : public Atom {
std::string value_;
public:
  void setValue(const std::string s) { value_ = s; }

  virtual std::string getValue() { return value_; }
};

class NumericAtom : public Atom {
int value_;
public:
  void setValue(std::string s);
  virtual std::string getValue();
  int getNumericValue();
};


class ListAtom : public Atom {
std::vector<Atom*> values;
public:
  void setValue(std::string s) {}

  virtual bool isTrue();

  void addValue(Atom* value);
  virtual std::string getValue();

  std::vector<Atom*>& getValues();
};


#endif
