
#include "expressions.h"
#include "factories.h"


Atom* Expression::getArgumentValue(Argument a, Context& c) {
  if (a.isAtom()) {
    return a.getAtom();
  } else {
    Expression* e = a.getExpression();
    return e->eval(c);
  }
}


Atom* PrintExpression::eval(Context& c) {

  deque<Argument> args_ = getArguments();

  deque<Argument>::iterator it = args_.begin();
  for (; it != args_.end(); ++it) {
    Atom* a = getArgumentValue(*it, c);
    cout << a->getValue() << " ";
  }
  cout << endl;

  NilAtom* nil = c.getAtomFactory().createNil();;
  return nil;
}


Atom* MathExpression::eval(Context& c) {
  deque<Argument> args_ = getArguments();

  deque<Argument>::iterator it = args_.begin();

  int value = stoi(getArgumentValue(*it, c)->getValue());

  for (++it; it != args_.end(); ++it) {
    Atom* a = getArgumentValue(*it, c);
    value = operation(value, stoi(a->getValue()));
  }

  stringstream ss;
  ss << value;
  StringAtom* result = c.getAtomFactory().createString();
  result->setValue(ss.str());
  return result;
}


Atom* ListExpression::eval(Context& c) {
  ListAtom* result = c.getAtomFactory().createList();

  deque<Argument> args_ = getArguments();

  deque<Argument>::iterator it = args_.begin();

  for (++it; it != args_.end(); ++it) {
    Atom* a = getArgumentValue(*it, c);
    result->addValue(a);
  }

  return result;
}

