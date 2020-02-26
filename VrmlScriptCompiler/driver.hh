#pragma once

#include <string>
#include <map>
#include "nodes.hh"
#include "parser.tab.h"

#define YY_DECL yy::parser::symbol_type yylex(driver& drv)

YY_DECL;

class driver
{
private:
	std::map<std::string, vrmlast::FunctionDefinition> m_functionDefinitions;
	
public:
	driver();

	vrmlast::ASTNode* m_root;
	std::map<std::string, int> variables;
	int result;

	int parse(const std::string& f);
	std::string file;
	bool trace_parsing;

	void scan_begin();
	void scan_end();

	bool trace_scanning;
	yy::location location;

	void add_function(std::string name, vrmlast::ArgumentList args, vrmlast::StatementList statements);
	void set_root(vrmlast::ASTNode* node) { m_root = node; }
};

std::ostream& operator<<(std::ostream out, vrmlast::ArgumentList args);