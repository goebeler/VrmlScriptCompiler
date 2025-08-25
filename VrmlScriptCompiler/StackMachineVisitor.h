#pragma once
#include <memory>
#include "nodes.hh"
#include "VrmlVariant.h"
#include "stack_machine.h"

namespace vrmlast
{
	class StackMachineVisitor : public ASTVisitor
	{
	public:
		StackMachineVisitor();
		// Geerbt über ASTVisitor
		void visit(ArgumentList* args) override;
		void visit(FunctionDefinition* func) override;
		void visit(ParameterList* params) override;
		void visit(FunctionCallExpression* call) override;
		void visit(Statement* statement) override;
		void visit(AssignmentExpression* a) override;
		void visit(VariableExpression* a) override;
		void visit(IntConstantExpression* a) override;
		void visit(StatementList* s) override;
		void visit(Expression* s) override;
		void visit(FunctionDefinitionList* s) override;
		void visit(Script* s) override;
		void visit(BinaryArithmeticExpression* s) override;
		void visit(Block* s) override;

		std::string to_string() { return "StackMachineVisitor.to_string()"; };

	private:
		std::unique_ptr<stackmachine::stack_machine> m_stackmachine;
	};
}