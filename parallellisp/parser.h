#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>

#include "expressions.h"
#include "atoms.h"
#include "factories.h"

enum ParsingContext {
  CommonExpression,
  Setq,
  Sync
};

class Parser {
  Context& globalContext_;
  ParsingContext parsingContext_;

  Expression* getExpressionInstance_(std::string name);

  Atom* getAtomInstance_(std::string s);

  Expression* parseExpression_(const std::string s);

public:
  explicit Parser(Context& globalContext);

  Expression* parse(const std::string s);

  ParsingContext getParsingContext();
};



#endif
