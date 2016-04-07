
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

  deque<Argument> args = getArguments();

  deque<Argument>::iterator it = args.begin();
  for (; it != args.end(); ++it) {
    Atom* a = getArgumentValue(*it, c);
    cout << a->getValue() << " ";
  }
  cout << endl;

  NilAtom* nil = c.getAtomFactory().createNil();;
  return nil;
}


Atom* MathExpression::eval(Context& c) {
  deque<Argument> args = getArguments();

  deque<Argument>::iterator it = args.begin();

  int value = stoi(getArgumentValue(*it, c)->getValue());

  for (++it; it != args.end(); ++it) {
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

  deque<Argument> args = getArguments();

  deque<Argument>::iterator it = args.begin();

  for (; it != args.end(); ++it) {
    Atom* a = getArgumentValue(*it, c);
    result->addValue(a);
  }

  return result;
}


Atom* CarExpression::eval(Context& c) {
  deque<Argument> args = getArguments();

  if (!args.size()) return c.getAtomFactory().createList();

  ListAtom* list = (ListAtom*) getArgumentValue(args.front(), c);

  vector<Atom*>& values = list->getValues();

  if (!values.size()) return c.getAtomFactory().createList();

  return extractAtom(values, c);

}


Atom* CarExpression::extractAtom(vector<Atom*> values, Context& c) {
  return values[0];
}


Atom* CdrExpression::extractAtom(vector<Atom*> values, Context& c) {
  ListAtom* result = c.getAtomFactory().createList();

  if (values.size() < 2) return result;

  vector<Atom*>::iterator it = values.begin() + 1;
  for (; it != values.end(); ++it) {
    result->addValue(*it);
  }

  return result;
}


Atom* AppendExpression::eval(Context& c) {
  ListAtom* result = c.getAtomFactory().createList();

  deque<Argument> args = getArguments();
  deque<Argument>::iterator it = args.begin();
  for (; it != args.end(); ++it) {
    ListAtom* atom = (ListAtom*) getArgumentValue(*it, c);;

    vector<Atom*>& values = atom->getValues();
    for (int i = 0; i < values.size(); ++i) {
      result->addValue(values[i]);
    }
  }

  return result;
}
