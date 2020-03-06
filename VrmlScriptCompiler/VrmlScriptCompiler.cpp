#include <iostream>
#include "driver.hh"
//#include "PrintASTVisitor.hh"
#include "GenerateCppVisitor.h"

int main(int argc, char *argv[])
{
	int res = 0;
	driver drv;
	for(int i = 1; i < argc; ++i)
	{
		if (argv[i] == std::string("-p"))
			drv.trace_parsing = true;
		else if (argv[i] == std::string("-s"))
			drv.trace_scanning = true;
		else if (!drv.parse(argv[i]))
			std::cout << drv.result << '\n';
		else
			res = 1;
	}
	vrmlast::GenerateCppVisitor printer;
	if(drv.m_root)
		drv.m_root->accept(printer);
	std::cout << printer.to_string();
	return res;
}
