#pragma once
#include "nodes.hh"
namespace stackmachine
{
	class stack_machine;
}

namespace vrmlast
{
	class evaluate_visitor : public ASTNode
	{
	public: 

		evaluate_visitor(Script* script);
		virtual void visit(ArgumentList* args);
		virtual void visit(FunctionDefinition* func);
		virtual void visit(ParameterList* params);
		virtual void visit(FunctionCallExpression* call);
		virtual void visit(Statement* statement);
		virtual void visit(AssignmentExpression* a);
		virtual void visit(VariableExpression* a);
		virtual void visit(IntConstantExpression* a);
		virtual void visit(StatementList* s);
		virtual void visit(Expression* s);
		virtual void visit(Script* s);
		virtual void visit(FunctionDefinitionList* s);
		virtual void visit(Block* s);
		virtual void visit(BinaryArithmeticExpression* s);

	private:
		stackmachine::stack_machine* m_stackmachine {nullptr};
		Script* m_script;
		std::map<std::string, vrmlscript::VrmlVariant> m_current_stack_variables;
	};
}
