#pragma once
#include <string>
#include <sstream>
#include "nodes.hh"

namespace vrmlast {
	class GenerateCppVisitor : public ASTVisitor
	{
	private:
		int m_indent = 0;
		std::stringstream m_out;
		void indent();
		void unindent();
		void print(std::string);
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

		std::string to_string() { return m_out.str(); };
	};
}