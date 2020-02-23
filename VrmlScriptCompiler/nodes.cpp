#include "nodes.hh"
namespace vrmlast {
	std::ostream& operator<<(std::ostream& out, const vrmlast::ArgumentList& args)
	{
		for (const auto& item : args)
		{
			out << item;
		}
		return out;
	}

	std::ostream& operator<<(std::ostream& out, const StatementList& statement_list)
	{
		for (const auto& item : statement_list)
		{
			out << item;
		}
		return out;
	}

	std::ostream& operator<<(std::ostream& out, const FunctionDefinition& function_definition)
	{
		out << "function " << function_definition.m_name
			<< "(" << function_definition.m_arguments << ") {'\n"
			<< function_definition.m_statements << "\n}";
		return out;
	}

	void ArgumentList::add_argument(std::string name)
	{
		push_back(name);
	}

	void Statement::add_expression(Expression exp)
	{
		push_back(std::forward<Expression>(exp));
	}

	void StatementList::add_statement(Statement statement)
	{
		push_back(std::forward<Statement>(statement));
	}

	void FunctionDefinitionList::add_function(FunctionDefinition func)
	{
		push_back(std::forward<FunctionDefinition>(func));
	}
}

