
%{
#ifdef _MSC_VER
	#define _CRT_SECURE_NO_WARNINGS
	#include <stdint.h>
#endif
#include <cerrno>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <string>
#include "driver.hh"
#include <parser.hpp>

#define RET(TOK)        do { drv.location = loc; return yy::parser::make_##TOK(loc); } while(0)
#define RETV(TOK,VAL)   do { drv.location = loc; return yy::parser::make_##TOK((VAL), loc); } while(0)
%}


%option noyywrap nounput noinput batch debug

%{
	yy::parser::symbol_type make_NUMBER(const std::string &s, const yy::parser::location_type& loc);
%}

id		[a-zA-Z][a-zA-Z_0-9]*
int		[0-9]+
blank	[ \t\r]

%{
	#define YY_USER_ACTION loc.columns(yyleng);
%}

%%
%{
	yy::location loc = drv.location;
	loc.step();
%}

{blank}+	loc.step();
\n+			loc.lines(yyleng); loc.step();
/* comments (kept without states) */
"//"[^\n]*                      /* skip single-line comment (also works after code) */
"/*"([^*]|\*+[^*/])*\*+"/"      { /* skip multi-line comment, count newlines */
    for (int i = 0; i < (int)yyleng; ++i)
        if (yytext[i] == '\n') loc.lines(1);
}

","		return yy::parser::make_COMMA (loc);
">>>"   return yy::parser::make_tRSHIFTFILL(loc);
">>="   return yy::parser::make_tRSHIFTEQ(loc);
"<<="   return yy::parser::make_tLSHIFTEQ(loc);
">>"    return yy::parser::make_tRSHIFT(loc);
"<<"    return yy::parser::make_tLSHIFT(loc);
">"		return yy::parser::make_tGT(loc);
">="	return yy::parser::make_tGE(loc);
"=="    return yy::parser::make_tEQ(loc);
"<"     return yy::parser::make_tLT(loc);
"<="	return yy::parser::make_tLE(loc);
"!="    return yy::parser::make_tNE(loc);
"&&"    return yy::parser::make_tLAND(loc);
"||"    return yy::parser::make_tLOR(loc);

"+="    return yy::parser::make_tPLUSEQ(loc);
"-="    return yy::parser::make_tMINUSEQ(loc);
"*="    return yy::parser::make_tMULTIPLYEQ(loc);
"/="    return yy::parser::make_tDIVIDEEQ(loc);
"%="    return yy::parser::make_tMODEQ(loc);

"="     return yy::parser::make_tASSIGN (loc);
"|"     return yy::parser::make_tOR(loc);       
"&"     return yy::parser::make_tAND(loc);      
"^"     return yy::parser::make_tXOR(loc);

"+"        return yy::parser::make_tPLUS   (loc);
"-"        return yy::parser::make_tMINUS  (loc);
"*"        return yy::parser::make_tMULTIPLY(loc);
"/"        return yy::parser::make_tDIVIDE  (loc);
"%"        return yy::parser::make_tMOD(loc);
"!"        return yy::parser::make_tNOT(loc);
"."        return yy::parser::make_tDOT(loc);
"("        return yy::parser::make_LPAREN (loc);
")"        return yy::parser::make_RPAREN (loc);
"{"        return yy::parser::make_LBRACK (loc);
"}"        return yy::parser::make_RBRACK (loc);


";"       return yy::parser::make_SEMICOLON (loc);
"function" return yy::parser::make_FUNCTION(loc);
"var"	   return yy::parser::make_VAR(loc);
"NULL"     return yy::parser::make_NULL(loc);

'([^'\\\n]|\\.|\n)*'	{
    std::string raw(yytext, yyleng);
    std::string inner = raw.substr(1, raw.size() - 2);
    // Advance location by the number of literal newlines inside the token
    for (char c : inner) if (c == '\n') loc.lines(1);
    RETV(STRING, inner);
}


{int}      return make_NUMBER (yytext, loc);
{id}       return yy::parser::make_IDENTIFIER (yytext, loc);

.          {
             throw yy::parser::syntax_error
               (loc, "invalid character: " + std::string(yytext));
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

void driver::scan_begin ()
{
	yy_flex_debug = trace_scanning;
	if (file.empty () || file == "-")
	{
		yyin = stdin;
	}
	else if (!(yyin = fopen(file.c_str (), "r")))
	{
		std::cerr << "cannot open " << file << ": " << strerror(errno) << '\n';
		exit (EXIT_FAILURE);
	}
}


void driver::scan_end ()
{
	if (yyin) fclose (yyin);
}
