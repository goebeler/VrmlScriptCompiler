#include "nodes.hh"
#include <numeric>
#include <fmt/format.h>
#include <variant>

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

	void ArgumentList::add_argument(Expression* e)
	{
		m_arguments.push_back(e);
	}

	std::string ArgumentList::to_string() const
	{
		std::string result;
		//return std::accumulate(begin(m_arguments), end(m_arguments), result);
		return result;
	}

	void ArgumentList::accept(ASTVisitor& visitor)
	{
		visitor.visit(this);
	}

	void Statement::add_expression(Expression* exp)
	{
		m_expressions.push_back(exp);
	}

	std::string Statement::to_string() const
	{
		std::string result;
		for (const auto& e : m_expressions)
		{
			result += e->to_string() + ' ';
		}
		result += ';';
		return result;
	}

	void Statement::accept(ASTVisitor& visitor)
	{
		visitor.visit(this);
	}

	void StatementList::add_statement(Statement* statement)
	{
		m_statements.push_back(statement);
	}

	std::string StatementList::to_string() const
	{
		std::string result;
		for (const auto& s : m_statements)
		{
			result += (*s).to_string() + '\n';
		}
		return result;
	}

	void StatementList::accept(ASTVisitor& visitor)
	{
		visitor.visit(this);
	}

	void FunctionDefinitionList::add_function(FunctionDefinition* func)
	{
		m_functions.push_back(func);
	}

	std::string FunctionDefinitionList::to_string() const
	{
		std::string result;
		for (const auto& f : m_functions)
		{
			result += (*f).to_string() + '\n';
		}
		return result;
	}

	void FunctionDefinitionList::accept(ASTVisitor& visitor)
	{
		visitor.visit(this);
	}

	void Expression::accept(ASTVisitor& visitor)
	{
		visitor.visit(this);
	}

	std::string FunctionDefinition::to_string() const
	{
		return "function " + m_name + "(" + m_parameter_list->to_string() + ")\n" + m_statement->to_string() + "\n";
	}

	void FunctionDefinition::accept(ASTVisitor& visitor)
	{
		visitor.visit(this);
	}

	std::string ParameterList::to_string() const
	{
		std::string result;
		for (const auto& p : m_parameters)
		{
			result += p + '\n';
		}
		return result;
	}

	void ParameterList::accept(ASTVisitor& visitor)
	{
	}

	void ParameterList::add_parameter(std::string name)
	{
		m_parameters.push_back(name);
	}
	
	std::string AssignmentExpression::to_string() const
	{
		std::string result(m_lhs->to_string());
		result += " = " + m_rhs->to_string();
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

	std::string FunctionCallExpression::to_string() const
	{
		return std::string();
	}

	void FunctionCallExpression::accept(ASTVisitor& visitor)
	{
		visitor.visit(this);
	}

	std::string Script::to_string() const
	{
		return std::string();
	}
	
	void Script::accept(ASTVisitor& visitor)
	{
		visitor.visit(this);
	}

	std::string BinaryArithmeticExpression::to_string() const
	{
		std::string operator_sign;
		switch (m_op)
		{
		case ArithmeticOperatorEnum::PLUS:
			operator_sign = "+";
			break;
		default:
			operator_sign = "ERROR";
			break;
		}
		return fmt::format("{0} {1} {2}", m_lhs->to_string(), operator_sign ,m_rhs->to_string());
	}
	void BinaryArithmeticExpression::accept(ASTVisitor& visitor)
	{
		visitor.visit(this);
	}

	struct add_visitor
	{
		vrmlscript::VrmlVariant operator()(vrmlscript::SFInt32 int_in, vrmlscript::SFString string_in) const
		{
			return vrmlscript::VrmlVariant{fmt::format("{0}{1}",int_in, string_in.to_string())};
		}

		vrmlscript::VrmlVariant operator()(vrmlscript::SFInt32 int_in, vrmlscript::SFInt32 int2_in) const
		{
			return vrmlscript::VrmlVariant{int_in + int2_in};
		}

		vrmlscript::VrmlVariant operator()(auto int_in, auto string_in)
		{
			return vrmlscript::VrmlVariant{};
		}
	} adder2;

	vrmlscript::VrmlVariant BinaryArithmeticExpression::evaluate()
	{
		/*const auto add = [](auto left, auto right)->vrmlscript::VrmlVariant
		{
			using TLEFT = std::decay_t<decltype(left)>;
			using TRIGHT = std::decay_t<decltype(right)>;

		};*/
		
		std::visit(adder2, m_lhs->evaluate(), m_rhs->evaluate());
		return vrmlscript::VrmlVariant{};
	}


	std::string Block::to_string() const
	{
		return fmt::format("{{\n{0}\n}}", this->m_statements->to_string());
	}

	void Block::accept(ASTVisitor& visitor)
	{
		visitor.visit(this);
	}
}

