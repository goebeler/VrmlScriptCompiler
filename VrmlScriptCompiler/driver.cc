#include "driver.hh"
#include "parser.tab.h"
#include <iostream>
#include <fstream>

driver::driver()
	:trace_parsing(true), trace_scanning(true)
{
	variables["one"] = 1;
	variables["two"] = 2;
}

int driver::parse(const std::string& f)
{
	{
		std::ifstream input(f);

		if (input.is_open())
			std::cout << input.rdbuf() << '\n';
	}
	
	file = f;
	location.initialize(&file);
	scan_begin();
	yy::parser parse(*this);
	parse.set_debug_level(trace_parsing);
	int res = parse();
	scan_end();
	return res;
}

void driver::add_function(std::string name, vrmlast::ArgumentList args, vrmlast::StatementList statements)
{
}


