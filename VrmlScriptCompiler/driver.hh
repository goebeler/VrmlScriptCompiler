#pragma once

#include <string>
#include <map>
#include "nodes.hh"
#include <parser.hpp>

#define YY_DECL yy::parser::symbol_type yylex(driver& drv)
YY_DECL;

struct yy_buffer_state; 
using YY_BUFFER_STATE = yy_buffer_state*;

class driver
{
private:
	std::map<std::string, vrmlast::FunctionDefinition> m_functionDefinitions;
	// Scanner-Lebenszyklus für String-Quelle
    void scan_string_begin(const char* data, int len);
    void scan_string_end();

    // Buffer-Handle für String-Scanning (nur gültig zwischen begin/end)
    YY_BUFFER_STATE string_buf_ = nullptr;
	
public:
	driver();

	vrmlast::ASTNode* m_root = nullptr;
	
	int result;

	int parse(const std::string& f);
	int parse_string(const std::string& source);

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