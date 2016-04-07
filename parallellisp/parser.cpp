
#include "parser.h"


Expression* Parser::getExpressionInstance_(string name) {
  ExpressionFactory& expFact = globalContext_.getExpressionFactory();
  if (name == "print") {
    return expFact.createPrint();
  } else if (name == "+") {
    return expFact.createSum();
  } else if (name == "-") {
    return expFact.createDiff();
  } else if (name == "*") {
    return expFact.createMul();
  } else if (name == "/") {
    return expFact.createDiv();
  } else if (name == "list") {
    return expFact.createList();
  } else if (name == "car") {
    return expFact.createCar();
  } else if (name == "cdr") {
    return expFact.createCdr();
  } else if (name == "append") {
    return expFact.createAppend();
  } else {
    return NULL;
  }
}


Atom* Parser::getAtomInstance_(string s) {
  AtomFactory& atomFact = globalContext_.getAtomFactory();
  StringAtom* a = atomFact.createString();
  a->setValue(s);
  return a;
}


Expression* Parser::parseExpression_(const string s) {
  if (!isExpression_(s)) return NULL;

  istringstream iss(s.substr(1, s.size() - 2));

  cout << "uso " << s.substr(1, s.size() - 2) << endl;

  string expressionName;
  iss >> expressionName;

  Expression* result = getExpressionInstance_(expressionName);
  if (result == NULL) return NULL;

  string token;

  while (iss >> token) {
    if (token[0] == '(') {
      string tokenAux;
      int bracketCount = 0;

      bracketCount += count(token.begin(), token.end(), '(');
      bracketCount -= count(token.begin(), token.end(), ')');

      while (!iss.eof() && bracketCount) {
        iss >> tokenAux;

        bracketCount += count(tokenAux.begin(), tokenAux.end(), '(');
        bracketCount -= count(tokenAux.begin(), tokenAux.end(), ')');

        token.append(" ");
        token.append(tokenAux);
      }
      Expression* e = parseExpression_(token);
      if (e == NULL) return NULL;
      result->addArgument(e);
    } else {
      Atom* atom = getAtomInstance_(token);
      result->addArgument(atom);
    }
    token.clear();
  }
  return result;

}
