%skeleton "lalr1.cc"
%require  "3.0"
%debug
%defines
%define api.namespace {serpentine}
%define parser_class_name {Parser}
%locations

%code requires{
   namespace serpentine {
      class Scanner;
   }
}

%parse-param { Scanner  &scanner  }

%code{
   #include <iostream>
   #include <cstdlib>
   #include <fstream>

   #include "serp.h"

#undef yylex
#define yylex scanner.yylex
}

%define api.value.type variant
%define parse.assert

%token
  END  0  "end of file"
  BIND    "<-"
  ARROW   "=>"
  MINUS   "-"
  PLUS    "+"
  STAR    "*"
  SLASH   "/"
  COLON   ":"
  SCOLON  ";"
  LPAREN  "("
  RPAREN  ")"
  LBRACE  "["
  RBRACE  "]"
  LCURLY  "{"
  RCURLY  "}"
  IMPORT  "import"
  RETURN  "return"
  NIL     "nil"
  <std::string> ATOM
  <std::string> STRING
  <std::string> NUMBER
;

%locations

%%

program : statements END ;

statements :
           | statements statement SCOLON
           ;

statement  :
           | RETURN expr { scanner.print("return"); }
           | ATOM BIND expr { scanner.print("bind"); }
           ;

expr       :
           | ATOM { scanner.print("atom"); }
           | NIL { scanner.print("nil"); }
           | STRING { scanner.print("string"); }
           | NUMBER { scanner.print("number"); }
           | LCURLY statements RCURLY { scanner.print("expr"); }
           | LPAREN ATOM RPAREN ARROW expr { scanner.print("func"); }
           ;

%%

void serpentine::Parser::error(const location_type &l,
                              const std::string &err_message) {
   std::cerr << "Error: " << err_message << " at " << l << "\n";
}
