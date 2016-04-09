
#include <string>
#include <sstream>
#include <vector>

#include "atoms.h"


void NumericAtom::setValue(std::string s) {
  std::stringstream ss(s);
  ss >> value_;
}

std::string NumericAtom::getValue() {
  std::stringstream ss;
  ss << value_;
  return ss.str();
}

int NumericAtom::getNumericValue() {
  return value_;
}



bool ListAtom::isTrue() {
  return values.size();
}

void ListAtom::addValue(Atom* value) {
  values.push_back(value);
}

std::string ListAtom::getValue() {
  std::stringstream ss;
  ss << "(";
  for (size_t i = 0; i < values.size(); ++i) {
    ss << (values[i])->getValue();
    if (i != values.size() - 1) ss << " ";
  }
  ss << ")";

  return ss.str();
}

std::vector<Atom*>& ListAtom::getValues() {
  return values;
}
