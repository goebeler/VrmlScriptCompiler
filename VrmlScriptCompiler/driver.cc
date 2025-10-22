#include "driver.hh"
#include <parser.hpp>
#include <iostream>
#include <fstream>


// Flex-Funktionen/Typen (nicht-reentrant Standard)
// Die Signaturen kommen aus dem von Flex erzeugten scanner.cpp
extern YY_BUFFER_STATE yy_scan_bytes (const char* , int );
extern void            yy_delete_buffer(YY_BUFFER_STATE);
extern void            yy_switch_to_buffer(YY_BUFFER_STATE);
extern void            yyrestart (FILE * input_file );
extern FILE*           yyin;

driver::driver()
	:trace_parsing(true), trace_scanning(true)
{
}

int driver::parse(const std::string& file_path)
{
	{
		std::ifstream input(file_path);

		if (input.is_open())
			std::cout << input.rdbuf() << '\n';
	}
	
	file = file_path;
	location.initialize(&file);

	scan_begin();
	yy::parser parse(*this);
	parse.set_debug_level(trace_parsing);

	int res = parse();
	scan_end();

	return res;
}

int driver::parse_string(const std::string& source)
{
    // Für Location/Fehlerausgabe eine "virtuelle" Datei setzen
    file = std::string("<memory>");
    location.initialize(&file);

    // String als Eingabe-Buffer an Flex binden
    scan_string_begin(source.data(), static_cast<int>(source.size()));

    yy::parser parse(*this);
    parse.set_debug_level(trace_parsing);
    int res = parse();

    scan_string_end();
    return res;
}

void driver::add_function(std::string name, vrmlast::ArgumentList args, vrmlast::StatementList statements)
{
}




