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
LPAREN  "("
RPAREN  ")"
LBRACK  "{"
RBRACK  "}"
COMMA   ","
SEMICOLON ";"
FUNCTION "function"
VAR	  "var"
NULL  "null"
;

%token IF "if"
%token ELSE "else"
%token WHILE "while"
%token RETURN "return"
%token NEW "new"

%token <std::string> IDENTIFIER "identifier"
%token <int> NUMBER "number"
%token <std::string> STRING "string"

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

%start unit;

%nterm <vrmlast::IntConstantExpression*> intConstant
%nterm <vrmlast::FunctionDefinitionList*> functions
%nterm <vrmlast::FunctionDefinition*> function
%nterm <vrmlast::Expression*> exp 
%nterm <vrmlast::Expression*> primary
%nterm <vrmlast::Expression*> postfix
%nterm <vrmlast::LValueExpression*> leftexp
%nterm <vrmlast::Statement*> statement
%nterm <vrmlast::StatementList*> statements
%nterm <vrmlast::Block*> statement_block
%nterm <vrmlast::ArgumentList*> arguments
%nterm <vrmlast::ParameterList*> parameters
%nterm <vrmlast::AssignmentExpression*> assignment
%nterm <vrmlast::VariableExpression*> variable_reference
%nterm <vrmlast::VariableDeclarationExpression*> variable_decl
%nterm <vrmlast::Script*> unit
%nterm <vrmlast::Statement*> if_statement
%nterm <vrmlast::Statement*> while_statement
%nterm <vrmlast::Statement*> return_statement
%nterm <vrmlast::CompoundAssignOperatorEnum> comp_assign_op

/* Specific pretty-printer for the compound-assign enum (uses our C++20 mapper). */
%printer { yyo << std::string(vrmlast::CompoundAssignmentExpression::op_to_string($$)); } <vrmlast::CompoundAssignOperatorEnum>
/* Fallback for everything else that already has operator<< (pointers print fine). */
%printer { yyo << $$; } <*>;


%%


unit: 
	functions			{ $$ = new vrmlast::Script(); $$->m_functions = $1; drv.set_root($$); }
	;

functions:
	functions function	{ $1->m_functions.push_back($2); $$ = $1; }
	| function			{ $$ = new vrmlast::FunctionDefinitionList(); $$->m_functions.push_back($1); }
	| %empty			{ $$ = new vrmlast::FunctionDefinitionList(); }
	;

function:
	FUNCTION IDENTIFIER LPAREN  parameters RPAREN statement_block{
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
	statement_block				{ $$ = $1;}
	|exp SEMICOLON				{ $$ = new vrmlast::Statement();  $$->add_expression($1); }	
	|variable_decl SEMICOLON	{ $$ = new vrmlast::Statement(); $$->add_expression($1); }
	| if_statement              { $$ = $1; }
	| while_statement           { $$ = $1; }
	| return_statement          { $$ = $1; }
	;

return_statement:
	RETURN exp SEMICOLON        { auto r = new vrmlast::ReturnStatement(); r->m_expression = $2; $$ = r; }
	;


variable_decl:
	VAR IDENTIFIER					{ $$ = new vrmlast::VariableDeclarationExpression();  $$->m_name = $2;}
	| VAR IDENTIFIER tASSIGN exp	{ $$ = new vrmlast::VariableDeclarationExpression();  $$->m_name = $2; $$->m_initializer = $4; }
	;

variable_reference:
	IDENTIFIER			{ $$ = new vrmlast::VariableExpression();  $$->m_name = $1; }
	;

/* L-values for assignment: x, x.y, x[expr], chained */
leftexp:
	variable_reference	{$$ = $1;}
	| leftexp LBRACK exp RBRACK            { auto ie = new vrmlast::IndexExpression(); ie->m_collection_expression = $1,ie->m_index_expression = $3; $$=ie; }
	| leftexp tDOT IDENTIFIER              { auto mae = new vrmlast::MemberAccessExpression(); mae->m_object = $1; mae->m_member_name = $3; $$ = mae; }
	;

/* Assignment (lowest precedence, right-associative) */
assignment:
	leftexp tASSIGN exp			{ $$ = new vrmlast::AssignmentExpression();  $$->m_lhs = $1; $$->m_rhs = $3; }
	/* compound assignments as first-class AST node */
	| leftexp comp_assign_op exp
	  {
		auto n = new vrmlast::CompoundAssignmentExpression();
		n->m_lhs = $1;
		n->m_rhs = $3;
		n->m_op  = $2;
		$$ = n;
	  }
	;

/* Map tokens to CompoundAssignOperatorEnum (extend here if you add more) */
comp_assign_op:
	  tPLUSEQ         { $$ = vrmlast::CompoundAssignOperatorEnum::PLUS_EQ; }
	| tMINUSEQ        { $$ = vrmlast::CompoundAssignOperatorEnum::MINUS_EQ; }
	| tMULTIPLYEQ     { $$ = vrmlast::CompoundAssignOperatorEnum::MULTIPLY_EQ; }
	| tDIVIDEEQ       { $$ = vrmlast::CompoundAssignOperatorEnum::DIVIDE_EQ; }
	| tMODEQ          { $$ = vrmlast::CompoundAssignOperatorEnum::MOD_EQ; }
	| tLSHIFTEQ       { $$ = vrmlast::CompoundAssignOperatorEnum::LSHIFT_EQ; }
	| tRSHIFTEQ       { $$ = vrmlast::CompoundAssignOperatorEnum::RSHIFT_EQ; }
	| tRSHIFTFILLEQ   { $$ = vrmlast::CompoundAssignOperatorEnum::RSHIFTFILL_EQ; }
	| tANDEQ          { $$ = vrmlast::CompoundAssignOperatorEnum::AND_EQ; }
	| tXOREQ          { $$ = vrmlast::CompoundAssignOperatorEnum::XOR_EQ; }
	| tOREQ           { $$ = vrmlast::CompoundAssignOperatorEnum::OR_EQ; }
	;

if_statement:
	IF LPAREN exp RPAREN statement ELSE statement
	{ auto n = new vrmlast::IfStatement(); n->m_condition = $3; n->m_then = static_cast<vrmlast::Block*>($5); n->m_else = static_cast<vrmlast::Block*>($7); $$ = n; }
	| IF LPAREN exp RPAREN statement
	{ auto n = new vrmlast::IfStatement(); n->m_condition = $3; n->m_then = static_cast<vrmlast::Block*>($5); n->m_else = nullptr; $$ = n; }
	;

while_statement:
	WHILE LPAREN exp RPAREN statement
	{ auto n = new vrmlast::WhileStatement(); n->m_condition = $3; n->m_body = static_cast<vrmlast::Block*>($5); $$ = n; }
	;

arguments: 
	arguments COMMA exp	{ $1->add_argument($3); $$ = $1;}
	| exp					{ $$ = new vrmlast::ArgumentList();  $$->add_argument($1); }
	| %empty				{ $$ = new vrmlast::ArgumentList(); }
;

intConstant:
	NUMBER					{ $$ = new vrmlast::IntConstantExpression();  $$->set_value($1); }
	;

/* Primary expressions (atoms) */
primary:
      intConstant								{ $$ = $1; }
    | variable_reference						{ $$ = $1; }
    | LPAREN exp RPAREN							{ $$ = $2; }
	| STRING									{ auto s = new vrmlast::StringConstantExpression(); s->m_value = $1; $$ = s; }	
	| NULL                  					{ $$ = new vrmlast::NullLiteralExpression(); }
	| NEW IDENTIFIER LPAREN arguments RPAREN	{ auto n = new vrmlast::NewExpression(); n->m_type_name = $2; n->m_arguments = $4; $$ = n; }
;

/* Postfix chain for member/index/call in expressions */
postfix:
      primary                              { $$ = $1; }
    | postfix LBRACK exp RBRACK            { auto ie = new vrmlast::IndexExpression(); ie->m_collection_expression = $1; ie->m_index_expression = $3; $$ = ie;}
	| postfix tDOT IDENTIFIER              { auto mae = new vrmlast::MemberAccessExpression(); mae->m_object = $1; mae->m_member_name = $3; $$ = mae; }
	/* 1) Member call: obj.name(args) */
    | postfix tDOT IDENTIFIER LPAREN arguments RPAREN
      { auto call = new vrmlast::FunctionCallExpression();
        call->m_this = $1;
        call->m_function_name = $3;
        call->m_argument_list = $5;
        $$ = call; }
    /* 2) Free-standing call: name(args) */
    | IDENTIFIER LPAREN arguments RPAREN
      { auto call = new vrmlast::FunctionCallExpression();
        call->m_function_name = $1;
        call->m_argument_list = $3;
        $$ = call; }
;

exp:
	assignment					{ $$ = $1; }
	| postfix					{ $$ = $1; }
	/* Logical OR / AND */
	| exp tLOR exp              { $$ = new vrmlast::BinaryLogicalExpression(vrmlast::LogicalOperatorEnum::LOR, $1, $3); }
	| exp tLAND exp             { $$ = new vrmlast::BinaryLogicalExpression(vrmlast::LogicalOperatorEnum::LAND, $1, $3); }
	/* Equality / Relational */
	| exp tEQ exp               { $$ = new vrmlast::BinaryRelationalExpression(vrmlast::RelationalOperatorEnum::EQ, $1, $3); }
	| exp tNE exp               { $$ = new vrmlast::BinaryRelationalExpression(vrmlast::RelationalOperatorEnum::NE, $1, $3); }
	| exp tLT exp               { $$ = new vrmlast::BinaryRelationalExpression(vrmlast::RelationalOperatorEnum::LT, $1, $3); }
	| exp tLE exp               { $$ = new vrmlast::BinaryRelationalExpression(vrmlast::RelationalOperatorEnum::LE, $1, $3); }
	| exp tGT exp               { $$ = new vrmlast::BinaryRelationalExpression(vrmlast::RelationalOperatorEnum::GT, $1, $3); }
	| exp tGE exp               { $$ = new vrmlast::BinaryRelationalExpression(vrmlast::RelationalOperatorEnum::GE, $1, $3); }
	| tNOT exp                  { $$ = new vrmlast::UnaryNotExpression($2); }
	/*Multiplicative*/
	| exp tMULTIPLY exp			{$$ = new vrmlast::BinaryArithmeticExpression(vrmlast::ArithmeticOperatorEnum::MULTIPLY, $1, $3); }
	| exp tDIVIDE exp			{$$ = new vrmlast::BinaryArithmeticExpression(vrmlast::ArithmeticOperatorEnum::DIVIDE, $1, $3); }
	| exp tMOD exp			{$$ = new vrmlast::BinaryArithmeticExpression(vrmlast::ArithmeticOperatorEnum::MOD, $1, $3); }
	/*Additive*/
	| exp tPLUS exp			{$$ = new vrmlast::BinaryArithmeticExpression(vrmlast::ArithmeticOperatorEnum::PLUS, $1, $3); }
	| exp tMINUS exp			{$$ = new vrmlast::BinaryArithmeticExpression(vrmlast::ArithmeticOperatorEnum::MINUS, $1, $3); }
;

%%

void yy::parser::error(const location_type& l, const std::string& m)
{
	std::cerr << l << ": " << m << '\n';
}