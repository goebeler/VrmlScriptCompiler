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

		std::string to_string() { return m_out.str(); };
	};
}


