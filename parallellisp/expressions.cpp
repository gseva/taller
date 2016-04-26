
#include <deque>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "expressions.h"
#include "factories.h"


Argument::Argument(Atom* a): a_(a), isAtom_(true) {
}

Argument::Argument(Expression* e): e_(e), isAtom_(false) {
}

Argument::Argument(): isAtom_(false) {
}

Atom* Argument::getValue(Context &c) {
  if (isAtom_) {
    return a_;
  } else {
    return e_->eval(c);
  }
}

void Argument::setAtom(Atom* a) {
  a_ = a;
  isAtom_ = true;
}

void Argument::setExpression(Expression* e) {
  e_ = e;
  isAtom_ = false;
}


void Expression::addArgument(Expression* e) {
  Argument* arg = new Argument(e);
  args_.push_back(arg);
}

void Expression::addArgument(Atom* a) {
  Argument* arg = new Argument(a);
  args_.push_back(arg);
}

std::deque<Argument*>& Expression::getArguments() {
  return args_;
}

ListAtom* Expression::createNil(Context& c) {
  return c.getAtomFactory().createObject<ListAtom>();
}

Expression::~Expression() {
  std::deque<Argument*>::iterator it = args_.begin();
  for (; it != args_.end();) {
    delete *it;
    it = args_.erase(it);
  }
}


Atom* PrintExpression::eval(Context& c) {
  std::deque<Argument*> args = getArguments();
  std::stringstream ss;

  std::deque<Argument*>::iterator it = args.begin();
  for (; it != args.end(); ++it) {
    Atom* a = (*it)->getValue(c);

    ss << a->getValue();
    if (it != args.end() - 1) {
      ss << " ";
    }
  }

  Lock l(c.getMutex());
  std::cout << ss.str() << std::endl;

  return createNil(c);
}


Atom* MathExpression::eval(Context& c) {
  std::deque<Argument*> args = getArguments();

  std::deque<Argument*>::iterator it = args.begin();

  int value = ((NumericAtom*) (*it)->getValue(c))->getNumericValue();

  for (++it; it != args.end(); ++it) {
    NumericAtom* a = (NumericAtom*) (*it)->getValue(c);
    value = operation(value, a->getNumericValue());
  }

  std::stringstream ss;
  ss << value;
  NumericAtom* result = c.getAtomFactory().createObject<NumericAtom>();
  result->setValue(ss.str());
  return result;
}


int SumExpression::operation(int a, int b) {
  return a + b;
}


int DiffExpression::operation(int a, int b) {
  return a - b;
}


int MulExpression::operation(int a, int b) {
  return a * b;
}


int DivExpression::operation(int a, int b) {
  return a / b;
}


Atom* EqualExpression::eval(Context& c) {
  std::deque<Argument*> args = getArguments();

  if (!args.size()) return createNil(c);

  std::deque<Argument*>::iterator it = args.begin();

  Atom* a = (*it)->getValue(c);
  Atom* b = (*(it + 1))->getValue(c);
  if (compare(a->getValue(), b->getValue())) {
    NumericAtom* result = c.getAtomFactory().createObject<NumericAtom>();
    result->setValue("1");
    return result;
  } else {
    return createNil(c);
  }
}

bool EqualExpression::compare(const std::string& a, const std::string& b) {
  return a == b;
}


bool LesserExpression::compare(const std::string& a, const std::string& b) {
  return a < b;
}


bool GreaterExpression::compare(const std::string& a, const std::string& b) {
  return a > b;
}


Atom* ListExpression::eval(Context& c) {
  ListAtom* result = createNil(c);

  std::deque<Argument*> args = getArguments();

  std::deque<Argument*>::iterator it = args.begin();

  for (; it != args.end(); ++it) {
    Atom* a = (*it)->getValue(c);
    result->addValue(a);
  }

  return result;
}


Atom* CarExpression::eval(Context& c) {
  std::deque<Argument*> args = getArguments();

  if (!args.size()) return createNil(c);

  ListAtom* list = (ListAtom*) args.front()->getValue(c);
  std::vector<Atom*>& values = list->getValues();
  if (!values.size()) return createNil(c);

  return extractAtom(values, c);
}


Atom* CarExpression::extractAtom(std::vector<Atom*>& values, Context& c) {
  return values[0];
}


Atom* CdrExpression::extractAtom(std::vector<Atom*>& values, Context& c) {
  ListAtom* result = createNil(c);

  if (values.size() < 2) return result;

  std::vector<Atom*>::iterator it = values.begin() + 1;
  for (; it != values.end(); ++it) {
    result->addValue(*it);
  }

  return result;
}


Atom* AppendExpression::eval(Context& c) {
  ListAtom* result = createNil(c);

  std::deque<Argument*> args = getArguments();
  std::deque<Argument*>::iterator it = args.begin();
  for (; it != args.end(); ++it) {
    ListAtom* atom = (ListAtom*) (*it)->getValue(c);;

    std::vector<Atom*>& values = atom->getValues();
    for (size_t i = 0; i < values.size(); ++i) {
      result->addValue(values[i]);
    }
  }

  return result;
}


Atom* IfExpression::eval(Context& c) {
  std::deque<Argument*> args = getArguments();
  std::deque<Argument*>::iterator it = args.begin();
  Atom* r;
  if ((*it)->getValue(c)->isTrue()) {
    r = (*(it + 1))->getValue(c);

  } else {
    r =  (*(it + 2))->getValue(c);
  }
  return r;
}


Atom* SetqExpression::eval(Context& c) {
  std::deque<Argument*> args = getArguments();
  std::deque<Argument*>::iterator it = args.begin();

  std::string key = (*it)->getValue(c)->getValue();
  c.setAtom(key, (*(it + 1))->getValue(c));

  return createNil(c);
}


Atom* SyncExpression::eval(Context& c) {
  c.joinThreads();
  return createNil(c);
}


Atom* DefunExpression::eval(Context& c) {
  std::deque<Argument*> args = getArguments();
  std::deque<Argument*>::iterator it = args.begin();

  std::string key = (*it)->getValue(c)->getValue();

  c.setExpression(key, this);

  return createNil(c);
}

void DefunExpression::setExpressionString(const std::string& s) {
  expression_ = s;
}

std::string DefunExpression::getExpressionString(const std::string& param) {
  std::string result = expression_;
  size_t index = 0;

  while (true) {
    index = result.find("ENV", index);
    if (index == std::string::npos) break;
    result.replace(index, 3, param, 0, param.size());

    index += 3;
  }

  return result;
}
