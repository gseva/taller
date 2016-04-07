#ifndef __PARSER_H__
#define __PARSER_H__

#include "common.h"
#include "expressions.h"
#include "atoms.h"
#include "factories.h"


class Parser {

private:

  ExpressionFactory expFact_;

  bool isExpression_(const string s) {
    return (s[0] == '(' && s[s.size() - 1] == ')');
  }

  Expression* parseExpression_(const string s) {
    if (!isExpression_(s)) return NULL;

    istringstream iss(s.substr(1, s.size() - 2));

    cout << "uso " << s.substr(1, s.size() - 2) << endl;

    string expressionName;
    iss >> expressionName;

    Expression* result = NULL;

    string item;
    if (expressionName == "print") {
      result = expFact_.createPrint();
    } else if (expressionName == "sum") {
      result = expFact_.createSum();
    } else {
      return NULL;
    }

    string token;
    while (iss >> token) {
      if (token[0] == '(') {
        string tokenAux;
        while (token[token.size() - 1] != ')' && !iss.eof()) {
          iss >> tokenAux;
          token.append(" ");
          token.append(tokenAux);
        }
        printf("%s\n", token.c_str());
        Expression* e = parseExpression_(token);
        if (e == NULL) return NULL;
        result->addArgument(e);
      } else {
        Atom atom(token);
        result->addArgument(atom);
      }
      token.clear();
    }
    return result;

  }

public:

  // Parser() : expFact_(ef) {
  // }

  Expression* parse(const string s) {
    return parseExpression_(s);
  }

};



#endif
