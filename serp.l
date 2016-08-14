%{
#include <string>
#include "serp.h"
#include "serp_parse.hh"

#undef  YY_DECL
#define YY_DECL int serpentine::Scanner::yylex( serpentine::Parser::semantic_type * const lval, serpentine::Parser::location_type *location )
#define yyterminate() return( token::END )

using token = serpentine::Parser::token;

/* update location on matching */
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);
#define YY_USER_INIT yylval = lval;

%}

%option debug
%option nodefault
%option yyclass="serpentine::Scanner"
%option noyywrap
%option c++

%%
"<-"                       { return token::BIND;   }
"=>"                       { return token::ARROW;  }
"-"                        { return token::MINUS;  }
"+"                        { return token::PLUS;   }
"*"                        { return token::STAR;   }
"/"                        { return token::SLASH;  }
":"                        { return token::COLON;  }
";"                        { return token::SCOLON; }
"("                        { return token::LPAREN; }
")"                        { return token::RPAREN; }
"["                        { return token::LBRACE; }
"]"                        { return token::RBRACE; }
"{"                        { return token::LCURLY; }
"}"                        { return token::RCURLY; }
"import"                   { return token::IMPORT; }
"return"                   { return token::RETURN; }
"nil"                      { return token::NIL;    }
[a-zA-Z][a-zA-Z0-9]*       {
                             yylval->build<std::string>(yytext);
                             return token::ATOM;
                           }
\"(\\.|[^"])*\"            {
                             yylval->build<std::string>(yytext);
                             return token::STRING;
                           }
[0-9][0-9]*(\.[0-9]*)?     {
                             yylval->build<std::string>(yytext);
                             return token::NUMBER;
                           }
[ \t\n]                    { }
%%
