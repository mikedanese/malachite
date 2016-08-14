#ifndef SERP_H_
#define SERP_H_

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "serp_parse.hh"
#include "location.hh"

namespace serpentine {

class Scanner : public yyFlexLexer {
 public:
  Scanner(std::istream *in) : yyFlexLexer(in) {
    loc = new serpentine::Parser::location_type();
  };

  using FlexLexer::yylex;

  virtual int yylex(serpentine::Parser::semantic_type *const lval,
                    serpentine::Parser::location_type *location);

  void print(std::string s) {
    std::cerr << s << "\n";
  };

 private:
  serpentine::Parser::semantic_type *yylval = nullptr;
  serpentine::Parser::location_type *loc = nullptr;
};
}
#endif
