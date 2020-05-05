#pragma once
#include "nodes.hh"
#include "VrmlVariant.h"

namespace vrmlast
{
	class AssignVisitor : public ASTVisitor
	{
	public:
		vrmlscript::VrmlVariant m_new_value;
		// Inherited via ASTVisitor
		virtual void visit(VariableExpression* a) override;

		virtual void visit(ArgumentList* args) override;
		virtual void visit(FunctionDefinition* func) override;
		virtual void visit(ParameterList* params) override;
		virtual void visit(FunctionCallExpression* call) override;
		virtual void visit(Statement* statement) override;
		virtual void visit(AssignmentExpression* a) override;
		
		virtual void visit(IntConstantExpression* a) override;
		virtual void visit(Block* a) override;
		virtual void visit(StatementList* s) override;
		virtual void visit(Expression* s) override;
		virtual void visit(Script* s) override;
		virtual void visit(FunctionDefinitionList* s) override;
		virtual void visit(BinaryArithmeticExpression* s) override;
	};
}
