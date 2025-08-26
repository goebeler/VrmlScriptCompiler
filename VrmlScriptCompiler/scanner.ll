%{
#include <cerrno>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include "vrml_parser.hpp"
#include "location.hh"
#include "VrmlScanner.h"
%}

%option c++ noyywrap nodefault nounput noinput batch debug
%option yyclass="VrmlScanner"

%{
    // C++-yylex Signatur, die Bison erwartet:
  #define YY_DECL yy::parser::symbol_type VrmlScanner::yylex(yy::location* yylloc)

  using token = yy::parser::token;
  using symbol_type = yy::parser::symbol_type;


  

  yy::parser::symbol_type make_NUMBER(const std::string& s,
                                      const yy::parser::location_type& loc);
%}

id		[a-zA-Z][a-zA-Z_0-9]*
int		[0-9]+
blank	[ \t\r]

%{
  // Für jede gelesene Lexem-Länge Spalten hochzählen:
  #define YY_USER_ACTION yylloc->columns(YYLeng());
%}
%%
%{
	auto& loc = *yylloc;
	loc.step();
%}

{blank}+	loc.step();
\n+			loc.lines(YYLeng()); loc.step();

"-"        return yy::parser::make_MINUS  (loc);
"+"        return yy::parser::make_PLUS   (loc);
"*"        return yy::parser::make_STAR   (loc);
"/"        return yy::parser::make_SLASH  (loc);
"("        return yy::parser::make_LPAREN (loc);
")"        return yy::parser::make_RPAREN (loc);
"{"        return yy::parser::make_LBRACK (loc);
"}"        return yy::parser::make_RBRACK (loc);
"="        return yy::parser::make_ASSIGN (loc);
","        return yy::parser::make_COMMA (loc);
";"        return yy::parser::make_SEMICOLON (loc);
"function" return yy::parser::make_FUNCTION(loc);

{int}      return make_NUMBER (YYText(), loc);
{id}       return yy::parser::make_IDENTIFIER (YYText(), loc);

.          {
             throw yy::parser::syntax_error
               (loc, std::string("invalid character: ") + YYText());
           }

<<EOF>>    return yy::parser::make_END (loc);
%%

yy::parser::symbol_type
make_NUMBER (const std::string &s, const yy::parser::location_type& loc)
{
  errno = 0;
  long n = strtol (s.c_str(), NULL, 10);
  if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE))
    throw yy::parser::syntax_error (loc, "integer is out of range: " + s);
  return yy::parser::make_NUMBER ((int) n, loc);
}