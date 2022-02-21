#include <string>
#include "GenerateCppVisitor.h"
#include <fmt/format.h>
#include <math.h>
//#include "ScriptBase.h"
#include "AssignVisitor.h"

namespace vrmlast
{
	void GenerateCppVisitor::indent()
	{
		m_indent += m_indent_width;
	}

	void GenerateCppVisitor::unindent()
	{
		m_indent = std::max(0, m_indent - m_indent_width);
	}

	GenerateCppVisitor::GenerateCppVisitor()
	{
	}

	void GenerateCppVisitor::visit(ArgumentList* args)
	{
	}

	void GenerateCppVisitor::visit(FunctionDefinition* func)
	{
		func->m_statement->accept(*this);
	}

	void GenerateCppVisitor::visit(ParameterList* params)
	{
	}

	void GenerateCppVisitor::visit(FunctionCallExpression* call)
	{
	}

	void GenerateCppVisitor::visit(Statement* statement)
	{
		for (const auto& expression : statement->m_expressions)
		{
			expression->accept(*this);
		}
	}

	void GenerateCppVisitor::visit(AssignmentExpression* a)
	{
		a->m_rhs->accept(*this);

		AssignVisitor assigner{};
		assigner.m_new_value = m_current_value;
		a->m_lhs->accept(assigner);
	}

	void GenerateCppVisitor::visit(VariableExpression* a)
	{
		m_current_value = a->m_value;
	}

	void GenerateCppVisitor::visit(IntConstantExpression* a)
	{
		m_current_value = a->m_value;
	}

	void GenerateCppVisitor::visit(StatementList* s)
	{
		for (const auto& statement : s->m_statements)
		{
			statement->accept(*this);
		}
	}

	void GenerateCppVisitor::visit(Expression* s)
	{
	}

	void GenerateCppVisitor::visit(FunctionDefinitionList* s)
	{
		
		//write forward declarations		
		for (const auto& function_definition : s->m_functions)
		{
			std::stringstream temp;
			for (int i = 0; i < function_definition->m_parameter_list->m_parameters.size(); i++)
			{
				if (i > 0)
					temp << ", ";
				temp << "VrmlVariant " << function_definition->m_parameter_list->m_parameters[i];
			}
			
			m_out << fmt::format(
				"VrmlVariant {0} ({1});"
				, function_definition->m_name
				, temp.str());
		}

		m_out << "\n";

		for (const auto& function_definition : s->m_functions)
		{
			function_definition->accept(*this);
		}
	}

	void GenerateCppVisitor::visit(Script* s)
	{
		m_out << R"CODE(
#include "VrmlVariant.h"
#include "ScriptBase.h"

namespace vrmlscript
{
	class DummyScript : ScriptBase
	{
	public:
)CODE";

		s->m_functions->accept(*this);

		m_out << R"CODE(
	}
})CODE";
	}

	void GenerateCppVisitor::visit(BinaryArithmeticExpression* s)
	{
		s->m_lhs->accept(*this);
		auto left_value = m_current_value;

		s->m_rhs->accept(*this);
		auto right_value = m_current_value;

		// m_current_value = vrmlscript::ScriptBase::ArithAdd(left_value, right_value);
	}

	void GenerateCppVisitor::visit(Block* s)
	{
		for(const auto& statement : s->m_statements->m_statements)
		{
			statement->accept(*this);
		}
	}
}
