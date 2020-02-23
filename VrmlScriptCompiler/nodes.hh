#pragma once
#include <vector>
#include <string>
#include <memory>
#include <iostream>

namespace vrmlast
{
	class Expression;

	class ArgumentList : public std::vector<std::string>
	{
	public:
		ArgumentList()
		{

		}

		void add_argument(std::string name);
		friend std::ostream& operator<<(std::ostream& out, const ArgumentList& args);
	};
	class Expression
	{
	public:
		std::string type;
		std::string value;
		friend std::ostream& operator<<(std::ostream& out, const Expression& expression)
		{
			out << "//TODO: print Expression";
			return out;
		}
	};
	
	class Statement : public std::vector<Expression>
	{
	public:
		void add_expression(Expression exp);
		friend std::ostream& operator<<(std::ostream& out, const Statement& statement)
		{
			out << "//TODO: print statement";
			return out;
		}
	};
	
	class StatementList : public std::vector<Statement>
	{
	public:
		void add_statement(Statement statement);
		friend std::ostream& operator<<(std::ostream& out, const StatementList& statement_list);
	};

	class FunctionDefinition
	{
	public:
		FunctionDefinition()
			//: m_name(std::forward<std::string>(name))
			//, m_arguments(std::forward<ArgumentList>(args))
			//, m_statements(std::forward<StatementList>(statements))
		{}
		void set_name(std::string name) { m_name = std::move(name); }
		void set_arguments(ArgumentList arguments) { m_arguments = std::move(arguments); }
		void set_statements(StatementList statements) { m_statements = std::move(statements); }
		
		friend std::ostream& operator<<(std::ostream& out, const FunctionDefinition& function_definition);
	private:
		std::string m_name;
		ArgumentList m_arguments;
		StatementList m_statements;
	};

	class FunctionDefinitionList : public std::vector<FunctionDefinition>
	{
	public:

		void add_function(FunctionDefinition func);
		
		friend std::ostream& operator<<(std::ostream& out, const FunctionDefinitionList& function_definitions)
		{
			for(const auto& func : function_definitions)
			{
				out << func << '\n';
			}
			return out;
		}
	};
	
	class Block {};	

	class ParameterList : public std::vector<std::string>
	{
	public:
		void add_parameter(Expression e){};
		friend std::ostream& operator<<(std::ostream& out, const ParameterList& parameter_list)
		{
			out << "//TODO: print ParameterList";
			return out;
		}
	};

	class FunctionCallExpression : public Expression
	{
	public:
		FunctionCallExpression(std::string functionName, ParameterList parameters) {}
	};
}
