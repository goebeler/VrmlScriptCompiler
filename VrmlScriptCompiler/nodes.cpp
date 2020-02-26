#include "nodes.hh"
#include <numeric>

namespace vrmlast {
	std::ostream& operator<<(std::ostream& out, const vrmlast::ArgumentList& args)
	{
		out << args.to_string();
		return out;
	}

	std::ostream& operator<<(std::ostream& out, const StatementList& statement_list)
	{
		out << statement_list.to_string();
		return out;
	}

	void ArgumentList::add_argument(std::string name)
	{
		m_arguments.push_back(name);
	}

	std::string ArgumentList::to_string() const
	{
		std::string result;
		return std::accumulate(begin(m_arguments), end(m_arguments), result);
	}

	void ArgumentList::accept(ASTVisitor& visitor)
	{
		visitor.visit(this);
	}

	void Statement::add_expression(Expression exp)
	{
		m_expressions.push_back(std::forward<Expression>(exp));
	}

	std::string Statement::to_string() const
	{
		std::string result;
		for (const auto& e : m_expressions)
		{
			result += e.to_string() + ' ';
		}
		result += ';';
		return result;
	}

	void Statement::accept(ASTVisitor& visitor)
	{
		visitor.visit(this);
	}

	void StatementList::add_statement(Statement statement)
	{
		m_statements.push_back(std::forward<Statement>(statement));
	}

	std::string StatementList::to_string() const
	{
		std::string result;
		for (const auto& s : m_statements)
		{
			result += s.to_string() + '\n';
		}
		return result;
	}

	void StatementList::accept(ASTVisitor& visitor)
	{
	}

	void FunctionDefinitionList::add_function(FunctionDefinition func)
	{
		m_functions.push_back(std::forward<FunctionDefinition>(func));
	}

	std::string FunctionDefinitionList::to_string() const
	{
		std::string result;
		for (const auto& f : m_functions)
		{
			result += f.to_string() + '\n';
		}
		return result;
	}

	void FunctionDefinitionList::accept(ASTVisitor& visitor)
	{
	}

	std::string Expression::to_string() const
	{
		return m_value;
	}

	void Expression::accept(ASTVisitor& visitor)
	{
	}

	std::string FunctionDefinition::to_string() const
	{
		return "function " + m_name + "(" + m_arguments.to_string() + ") {'\n" + m_statements.to_string() + "\n}";
	}

	void FunctionDefinition::accept(ASTVisitor& visitor)
	{
	}

	std::string ParameterList::to_string() const
	{
		std::string result;
		for (const auto& p : m_parameters)
		{
			result += p.to_string() + '\n';
		}
		return result;
	}

	void ParameterList::accept(ASTVisitor& visitor)
	{
	}

	void ParameterList::add_parameter(Expression e)
	{
		m_parameters.push_back(std::move(e));
	}
	
	std::string AssignmentExpression::to_string() const
	{
		std::string result(m_lhs.to_string());
		result += " = " + m_rhs.to_string();
		return result;
	}
	
	void AssignmentExpression::accept(ASTVisitor& visitor)
	{
		visitor.visit(this);
	}
	std::string VariableExpression::to_string() const
	{
		return m_name;
	}
	void VariableExpression::accept(ASTVisitor& visitor)
	{
		visitor.visit(this);
	}
	std::string IntConstantExpression::to_string() const
	{
		return std::to_string(m_value);
	}

	void IntConstantExpression::accept(ASTVisitor& visitor)
	{
		visitor.visit(this);
	}
}

