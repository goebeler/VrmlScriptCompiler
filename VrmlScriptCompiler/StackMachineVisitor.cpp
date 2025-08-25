#include "StackMachineVisitor.h"
#include "ScriptBase.h"
#include "stack_machine.h"
#include "load_instruction.h"
#include "vrml_variable.h"
#include "add_instruction.h"

namespace vrmlast
{
	StackMachineVisitor::StackMachineVisitor()
		:m_stackmachine(std::make_unique<stackmachine::stack_machine>())
	{
		/*stackmachine::stack_machine machine;
		machine.load(new stackmachine::add_instruction());
		machine.load(new stackmachine::load_instruction(new stackmachine::sffloat(1.0f)));
		machine.load(new stackmachine::add_instruction());
		machine.load(new stackmachine::load_instruction(new stackmachine::sffloat(17.5f)));
		machine.load(new stackmachine::load_instruction(new stackmachine::sffloat(52.0f)));

		machine.run();	*/
	}

	void StackMachineVisitor::visit(ArgumentList* args)
	{
	}

	void StackMachineVisitor::visit(FunctionDefinition* func)
	{
		m_stackmachine->start_function(func->m_name);
		func->m_statement->accept(*this);
	}

	void StackMachineVisitor::visit(ParameterList* params)
	{
	}

	void StackMachineVisitor::visit(FunctionCallExpression* call)
	{
	}

	void StackMachineVisitor::visit(Statement* statement)
	{
		for (const auto& expression : statement->m_expressions)
		{
			expression->accept(*this);
		}
	}

	void StackMachineVisitor::visit(AssignmentExpression* a)
	{
		a->m_rhs->accept(*this);

		a->m_lhs->accept(*this);
	}

	void StackMachineVisitor::visit(VariableExpression* a)
	{
	}

	void StackMachineVisitor::visit(IntConstantExpression* a)
	{
	}

	void StackMachineVisitor::visit(StatementList* s)
	{
		for (const auto& statement : s->m_statements)
		{
			statement->accept(*this);
		}
	}

	void StackMachineVisitor::visit(Expression* s)
	{
	}

	void StackMachineVisitor::visit(FunctionDefinitionList* s)
	{
		for(const auto& function : s->m_functions)
		{
			function->accept(*this);
		}
	}

	void StackMachineVisitor::visit(Script* s)
	{
		s->m_functions->accept(*this);
	}

	void StackMachineVisitor::visit(BinaryArithmeticExpression* s)
	{
	}

	void StackMachineVisitor::visit(Block* s)
	{
		for(const auto& statement : s->m_statements->m_statements)
		{
			statement->accept(*this);
		}
	}
}
