#pragma once
#include "nodes.hh"
#include "VrmlVariant.h"

namespace vrmlast
{
	class StackMachineVisitor : public ASTVisitor
	{
	public:
		StackMachineVisitor();
		// Geerbt über ASTVisitor
		void visit(ArgumentList *args) override;
		void visit(FunctionDefinition *func) override;
		void visit(ParameterList *params) override;
		void visit(FunctionCallExpression *call) override;
		void visit(Statement *statement) override;
		void visit(AssignmentExpression *a) override;
		void visit(VariableExpression *a) override;
		void visit(IntConstantExpression *a) override;
		void visit(StatementList *s) override;
		void visit(Expression *s) override;
		void visit(Script *s) override;
		void visit(FunctionDefinitionList *s) override;
		void visit(BinaryArithmeticExpression *s) override;
		void visit(Block *s) override;
		void visit(BinaryRelationalExpression *s) override;
		void visit(BinaryLogicalExpression *s) override;
		void visit(UnaryNotExpression *s) override;
		void visit(NullLiteralExpression *s) override;
		void visit(ReturnStatement *s) override;
		void visit(IfStatement *s) override;
		void visit(WhileStatement *s) override;
		void visit(NewExpression *s) override;
		void visit(CompoundAssignmentExpression *s) override;

		std::string to_string() { return "StackMachineVisitor.to_string()"; };
	};
}