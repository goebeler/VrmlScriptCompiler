%skeleton "lalr1.cc"
%require "3.5"
%defines
%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires{
	#include <string>
	#include <nodes.hh>
	class driver;		
}

%param {driver& drv}

%locations

%define parse.trace
%define parse.error verbose

%code {
#include "driver.hh"
}

%define api.token.prefix {TOK_}
%token
END  0  "end of file"
//ASSIGN  "="
//MINUS   "-"
//PLUS    "+"
//MULTIPLY    "*"
//DIVIDE   "/"
LPAREN  "("
RPAREN  ")"
LBRACK  "{"
RBRACK  "}"
COMMA   ","
SEMICOLON ";"
FUNCTION "function"
NEW
VAR	  "var"
NULL  "NULL"
;

//%left SEMICOLON
//%left ASSIGN
//%left PLUS

%token <std::string> IDENTIFIER "identifier"
%token <int> NUMBER "number"

%right  tASSIGN tPLUSEQ tMINUSEQ tMULTIPLYEQ tDIVIDEEQ tMODEQ tRSHIFTEQ tLSHIFTEQ tRSHIFTFILLEQ tANDEQ tXOREQ tOREQ
%left   tCONDTEST tCONDSEP
%left   tLOR
%left   tLAND
%left   tOR
%left   tXOR
%left   tAND
%left   tEQ tNE
%left   tLT tLE tGE tGT
%left   tRSHIFT tLSHIFT tRSHIFTFILL
%left   tPLUS tMINUS
%left   tMULTIPLY tDIVIDE tMOD
%right  tNOT tNEG tONESCOMP tINCREMENT tDECREMENT
%left   tDOT RBRACK LBRACK

%nterm <vrmlast::IntConstantExpression*> intConstant
%nterm <vrmlast::FunctionDefinitionList*> functions
%nterm <vrmlast::FunctionDefinition*> function
%nterm <vrmlast::Expression*> exp functioncall leftexp
%nterm <vrmlast::Statement*> statement
%nterm <vrmlast::StatementList*> statements
%nterm <vrmlast::Block*> statement_block
%nterm <vrmlast::ArgumentList*> arguments
%nterm <vrmlast::ParameterList*> parameters
%nterm <vrmlast::AssignmentExpression*> assignment
%nterm <vrmlast::VariableExpression*> variable_reference
%nterm <vrmlast::VariableDeclarationExpression*> variable_decl
%nterm <vrmlast::Script*> unit
%nterm <vrmlast::BinaryArithmeticExpression*> binArithExp



%printer { yyo << $$; } <*>;

%%
%start unit;

unit: 
	functions			{ $$ = new vrmlast::Script(); $$->m_functions = $1; drv.set_root($$); }
	;

functions:
	functions function	{ $1->m_functions.push_back($2); $$ = $1; }
	| function			{ $$ = new vrmlast::FunctionDefinitionList(); $$->m_functions.push_back($1); }
	| %empty			{ $$ = new vrmlast::FunctionDefinitionList(); }
	;

function:
	FUNCTION IDENTIFIER "(" parameters ")" statement{
														$$ = new vrmlast::FunctionDefinition();
														$$->set_name($2); 
														$$->set_arguments($4); 
														$$->set_statement($6); }
	;

parameters:
	parameters COMMA IDENTIFIER		{ $1->add_parameter($3); $$ = $1; }
	| IDENTIFIER				{ $$ = new vrmlast::ParameterList(); $$->add_parameter($1); }
	| %empty					{ $$ = new vrmlast::ParameterList(); }
	;
	
statement_block:
	LBRACK statements RBRACK	{ $$ = new vrmlast::Block(); $$->m_statements = $2; }
	;

statements:
	statements statement	{ $1->add_statement($2); $$ = $1;}
	| statement				{ $$ = new vrmlast::StatementList(); $$->add_statement($1); }
	| %empty				{ $$ = new vrmlast::StatementList(); }
	;

statement:
	statement_block			{$$ = $1;}
	|exp ";"				{$$ = new vrmlast::Statement();  $$->add_expression($1); }	
	;

variable_decl:
	VAR IDENTIFIER		{$$ = = new vrmlast::VariableDeclarationExpression();  $$->m_name = $2;}

variable_reference:
	IDENTIFIER			{$$ = new vrmlast::VariableExpression();  $$->m_name = $1; }
	;

leftexp:
	variable_reference	{$$ = $1;}
	;

assignment:
	leftexp "=" exp			{ $$ = new vrmlast::AssignmentExpression();  $$->m_lhs = $1; $$->m_rhs = $3; }
	;

arguments: 
	arguments COMMA exp	{ $1->add_argument($3); $$ = $1;}
	| exp					{ $$ = new vrmlast::ArgumentList();  $$->add_argument($1); }
	| %empty				{ $$ = new vrmlast::ArgumentList(); }


functioncall:
	IDENTIFIER "(" arguments ")" ";" {}
	;

%left "+" "-";
%left "*" "/";

intConstant:
	"number"					{ $$ = new vrmlast::IntConstantExpression();  $$->set_value($1); }
	;

binArithExp:
	exp "+" exp					{
									$$ = new vrmlast::BinaryArithmeticExpression();
									$$->m_lhs = $1;
									$$->m_rhs = $3;
									$$->m_op = vrmlast::ArithmeticOperatorEnum::PLUS;
								}
//	| exp "-" exp				{$$ = $1 - $3;}
//	| exp "*" exp				{$$ = $1 * $3;}
//	| exp "/" exp				{$$ = $1 / $3;}

exp:
	intConstant					{ $$ = $1; }
	| assignment				{ $$ = $1; }
	| binArithExp				{ $$ = $1; }
	| variable_reference		{ $$ = $1; }
	| "(" exp ")"				{ $$ = $2;}
	| functioncall				{ $$ = $1;}
//	| %empty					{}
%%

void yy::parser::error(const location_type& l, const std::string& m)
{
	std::cerr << l << ": " << m << '\n';
}