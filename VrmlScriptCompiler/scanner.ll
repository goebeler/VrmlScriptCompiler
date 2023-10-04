%{
#include <cerrno>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <string>
#include "driver.hh"
#include <parser.hpp>
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

%x COMMENT  
	/* Define COMMENT as a start condition for multi-line comments */

%%
%{
	yy::location loc = drv.location;
	loc.step();
%}

{blank}+	loc.step();
\n+			loc.lines(yyleng); loc.step();
\/\/[^\n]*   /* Ignore single-line comments */

"\/\*"     /* Begin multi-line comment */ BEGIN(COMMENT);
<COMMENT>[^\n]*  /* Ignore everything inside a multi-line comment */
<COMMENT>\*\/  /* End multi-line comment */ BEGIN(INITIAL);  /* Return to the initial state */

","		return yy::parser::make_COMMA (loc);
">"		return yy::parser::make_tGT(loc);
">="	return yy::parser::make_tGE(loc);
"=="    return yy::parser::make_tEQ(loc);
"<"     return yy::parser::make_tLT(loc);
"<="	return yy::parser::make_tLE(loc);
"!="    return yy::parser::make_tNE(loc);
">>"    return yy::parser::make_tRSHIFT(loc);       
"<<"    return yy::parser::make_tLSHIFT(loc);       
">>>"   return yy::parser::make_tRSHIFTFILL(loc);   
"&&"    return yy::parser::make_tLAND(loc);
"||"    return yy::parser::make_tLOR(loc);

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
	fclose (yyin);
}
