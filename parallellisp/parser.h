#ifndef __PARSER_H__
#define __PARSER_H__

#include "common.h"
#include "expressions.h"
#include "atoms.h"
#include "factories.h"


class Parser {

  Context& globalContext_;

  bool isExpression_(const string s) {
    return (s[0] == '(' && s[s.size() - 1] == ')');
  }

  Expression* getExpressionInstance_(string name);

  Atom* getAtomInstance_(string s);

  Expression* parseExpression_(const string s);

public:

  Parser(Context& globalContext) : globalContext_(globalContext) {
  }

  Expression* parse(const string s) {
    return parseExpression_(s);
  }

};



#endif
