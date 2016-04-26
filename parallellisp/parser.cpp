
#include <algorithm>
#include <string>
#include <iostream>
#include <sstream>

#include "parser.h"


bool isNumber(const std::string& s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

bool isExpression_(const std::string& s) {
  return (s[0] == '(' && s[s.size() - 1] == ')');
}


Parser::Parser(Context& globalContext) : globalContext_(globalContext) {
}

Expression* Parser::parse(const std::string& s) {
  parsingContext_ = CommonExpression;
  return parseExpression_(s);
}

ParsingContext Parser::getParsingContext() {
  return parsingContext_;
}


Expression* Parser::getExpressionInstance_(const std::string& name) {
  Factory<Expression>& expFact = globalContext_.getExpressionFactory();

  if (name == "print") {
    return expFact.createObject<PrintExpression>();
  } else if (name == "+") {
    return expFact.createObject<SumExpression>();
  } else if (name == "-") {
    return expFact.createObject<DiffExpression>();
  } else if (name == "*") {
    return expFact.createObject<MulExpression>();
  } else if (name == "/") {
    return expFact.createObject<DivExpression>();
  } else if (name == "=") {
    return expFact.createObject<EqualExpression>();
  } else if (name == "<") {
    return expFact.createObject<LesserExpression>();
  } else if (name == ">") {
    return expFact.createObject<GreaterExpression>();
  } else if (name == "list") {
    return expFact.createObject<ListExpression>();
  } else if (name == "car") {
    return expFact.createObject<CarExpression>();
  } else if (name == "cdr") {
    return expFact.createObject<CdrExpression>();
  } else if (name == "append") {
    return expFact.createObject<AppendExpression>();
  } else if (name == "if") {
    return expFact.createObject<IfExpression>();
  } else if (name == "setq") {
    parsingContext_ = Setq;
    return expFact.createObject<SetqExpression>();
  } else if (name == "sync") {
    parsingContext_ = Sync;
    return expFact.createObject<SyncExpression>();
  } else if (name == "defun") {
    parsingContext_ = Defun;
    return expFact.createObject<DefunExpression>();
  } else {
    Expression* e = globalContext_.getExpression(name);
    if (e != NULL) {
      parsingContext_ = Fun;
    }
    return e;
  }
}


Atom* Parser::getAtomInstance_(const std::string& s) {
  Factory<Atom>& atomFact = globalContext_.getAtomFactory();

  if (isNumber(s)) {
    NumericAtom* a = atomFact.createObject<NumericAtom>();
    a->setValue(s);
    return a;
  }

  if (parsingContext_ != Setq) {
    Atom* variable = globalContext_.getAtom(s);
    if (variable != NULL) return variable;
  }

  StringAtom* a = atomFact.createObject<StringAtom>();
  a->setValue(s);
  return a;
}


Expression* Parser::functionExpression_(Expression* r, const std::string& s) {
  std::string newExpr = ((DefunExpression*) r)->getExpressionString(s);
  parsingContext_ = CommonExpression;
  return parseExpression_(newExpr);
}


Expression* Parser::parseExpression_(const std::string& s) {
  if (!isExpression_(s)) return NULL;

  std::istringstream iss(s.substr(1, s.size() - 2));

  std::string expressionName;
  iss >> expressionName;

  Expression* result = getExpressionInstance_(expressionName);
  if (result == NULL) return NULL;

  std::string token;

  while (iss >> token) {
    if (token[0] == '(') {
      std::string tokenAux;
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

      if (parsingContext_ == Defun) {
        if (token == "(ENV)") {
          continue;
        } else {
          ((DefunExpression*) result)->setExpressionString(token);
          if (!iss.eof()) return NULL;
          return result;
        }
      }

      if (parsingContext_ == Fun) {
        return functionExpression_(result, token);
      }

      Expression* e = parseExpression_(token);
      if (e == NULL) return NULL;

      result->addArgument(e);
    } else {
      if (parsingContext_ == Fun) {
        return functionExpression_(result, token);
      }

      Atom* atom = getAtomInstance_(token);

      result->addArgument(atom);
    }
    token.clear();
  }

  return result;
}
