#ifndef __PARSER_H__
#define __PARSER_H__

#include "common.h"
#include "base.h"
#include "expressions.h"
#include "expressionFactory.h"

class Parser {

private:

  ExpressionFactory& expFact_;

  Expression* parseEnclosing_(const string s, int start, int end) {
    if (s[start] != '(' or s[end] != ')') return NULL;

    istringstream iss(s.substr(1, s.size() - 2));

    string expressionName;
    iss >> expressionName;

    if (expressionName == "print") {
      Print* p = expFact_.createPrint();
      string item;
      iss >> item;
      Atom atom(item);
      Argument arg(atom);
      p->addArgument(arg);
      return p;
    } else {
      return NULL;
    }

  }

public:

  Parser(ExpressionFactory& ef) : expFact_(ef) {
  }

  Expression* parse(const string s) {
    return parseEnclosing_(s, 0, s.size() - 1);
  }

};



#endif
