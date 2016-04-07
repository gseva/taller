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

  Expression* getExpressionInstance_(string name) {
    if (name == "print") {
      return expFact_.createPrint();
    } else if (name == "+") {
      return expFact_.createSum();
    } else if (name == "-") {
      return expFact_.createDiff();
    } else if (name == "*") {
      return expFact_.createMul();
    } else if (name == "/") {
      return expFact_.createDiv();
    } else {
      return NULL;
    }
  }

  Expression* parseExpression_(const string s) {
    if (!isExpression_(s)) return NULL;

    istringstream iss(s.substr(1, s.size() - 2));

    cout << "uso " << s.substr(1, s.size() - 2) << endl;

    string expressionName;
    iss >> expressionName;

    Expression* result = getExpressionInstance_(expressionName);
    if (result == NULL) return NULL;

    string item;

    string token;

    while (iss >> token) {
      if (token[0] == '(') {
        string tokenAux;
        int bracketCount = 1;
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
