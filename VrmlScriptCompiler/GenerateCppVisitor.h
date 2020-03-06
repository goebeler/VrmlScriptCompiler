#pragma once
#include "nodes.hh"
#include <sstream>
#include "VrmlVariant.h"

namespace vrmlast
{
	class GenerateCppVisitor : public ASTVisitor
	{
	private:
		int m_indent = 0;
		int m_indent_width = 4;
		std::stringstream m_out;
		vrmlscript::VrmlVariant m_current_value;
		void indent();
		void unindent();
	public:
		GenerateCppVisitor();
		// Geerbt über ASTVisitor
		virtual void visit(ArgumentList* args) override;
		virtual void visit(FunctionDefinition* func) override;
		virtual void visit(ParameterList* params) override;
		virtual void visit(FunctionCallExpression* call) override;
		virtual void visit(Statement* statement) override;
		virtual void visit(AssignmentExpression* a) override;
		virtual void visit(VariableExpression* a) override;
		virtual void visit(IntConstantExpression* a) override;
		virtual void visit(StatementList* s) override;
		virtual void visit(Expression* s) override;
		virtual void visit(FunctionDefinitionList* s) override;
		virtual void visit(Script* s) override;
		virtual void visit(BinaryArithmeticExpression* s) override;
		virtual void visit(Block* s) override;

		std::string to_string() { return m_out.str(); };
	};
}


