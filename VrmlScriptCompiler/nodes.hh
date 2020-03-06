#pragma once
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <iostream>
#include "VrmlVariant.h"

namespace vrmlast
{
	class ArgumentList;
	class FunctionDefinition;
	class ParameterList;
	class FunctionCallExpression;
	class Statement;
	class AssignmentExpression;
	class BinaryArithmeticExpression;
	class VariableExpression;
	class IntConstantExpression;
	class IntConstantExpression;
	class Expression;
	class StatementList;
	class Script;
	class FunctionDefinitionList;
	class Block;

	class ASTVisitor
	{
	public:
		virtual void visit(ArgumentList* args) = 0;
		virtual void visit(FunctionDefinition* func) = 0;
		virtual void visit(ParameterList* params) = 0;
		virtual void visit(FunctionCallExpression* call) = 0;
		virtual void visit(Statement* statement) = 0;
		virtual void visit(AssignmentExpression* a) = 0;
		virtual void visit(VariableExpression* a) = 0;
		virtual void visit(IntConstantExpression* a) = 0;
		virtual void visit(StatementList* s) = 0;
		virtual void visit(Expression* s) = 0;
		virtual void visit(Script* s) = 0;
		virtual void visit(FunctionDefinitionList* s) = 0;
		virtual void visit(Block* s) = 0;
		virtual void visit(BinaryArithmeticExpression* s) = 0;
	};

	class ASTNode
	{
	public:
		virtual ~ASTNode() {};
		virtual std::string to_string() const = 0;
		virtual void accept(ASTVisitor& visitor) = 0;
	};

	class Scope
	{
	public:
		std::map<std::string, std::unique_ptr<VariableExpression>> m_members;
		Scope* m_parent{ nullptr };
	};

	class Expression : public ASTNode
	{
	public:
		std::string m_type;
		//std::string m_value;

		// Geerbt über ASTNode
		virtual std::string to_string() const = 0;
		virtual void accept(ASTVisitor& visitor) = 0;
	};

	enum ArithmeticOperatorEnum
	{
		PLUS
	};

	class BinaryArithmeticExpression : public Expression
	{
	public:
		Expression* m_lhs;
		Expression* m_rhs;
		ArithmeticOperatorEnum m_op;

		// Geerbt über Expression
		virtual std::string to_string() const override;
		virtual void accept(ASTVisitor& visitor) override;
	};

	class Script : public ASTNode
	{
	public:
		FunctionDefinitionList* m_functions;
		Scope m_scope;
		// Geerbt über ASTNode
		virtual std::string to_string() const override;
		virtual void accept(ASTVisitor& visitor) override;
	};

	class VariableExpression : public Expression
	{
	public:
		VariableExpression() {}
		VariableExpression(std::string name, vrmlscript::VrmlVariant value = std::monostate{})
			:m_name{ name }, m_value{ value }
		{}

		std::string m_name;
		vrmlscript::VrmlVariant m_value;
		// Geerbt über ASTNode
		virtual std::string to_string() const override;
		virtual void accept(ASTVisitor& visitor) override;
	};

	class IntConstantExpression : public Expression
	{
	public:
		int m_value;

		void set_value(int value)
		{
			m_value = value;
		}
		// Geerbt über ASTNode
		virtual std::string to_string() const override;
		virtual void accept(ASTVisitor& visitor) override;
	};

	class ArgumentList : public ASTNode
	{
	public:
		std::vector<std::string> m_arguments;

		ArgumentList() {}

		void add_argument(std::string name);

		// Geerbt über ASTNode
		virtual std::string to_string() const override;
		virtual void accept(ASTVisitor& visitor) override;
	};

	class AssignmentExpression : public Expression
	{
	public:
		Expression* m_lhs;
		Expression* m_rhs;

		// Geerbt über ASTNode
		virtual std::string to_string() const override;
		virtual void accept(ASTVisitor& visitor) override;
	};

	class Statement : public ASTNode
	{
	public:
		std::vector<Expression*> m_expressions;
		void add_expression(Expression* exp);


		// Geerbt über ASTNode
		virtual std::string to_string() const override;
		virtual void accept(ASTVisitor& visitor) override;
	};

	class StatementList : public ASTNode
	{
	public:
		std::vector<Statement*> m_statements;
		void add_statement(Statement* statement);
		friend std::ostream& operator<<(std::ostream& out, const StatementList& statement_list);

		// Geerbt über ASTNode
		virtual std::string to_string() const override;
		virtual void accept(ASTVisitor& visitor) override;
	};

	class FunctionDefinition : public ASTNode
	{
	public:
		std::string m_name;
		ArgumentList* m_arguments = nullptr;
		Statement* m_statement = nullptr;
		Scope m_scope;

		FunctionDefinition() {}
		void set_name(std::string name) { m_name = std::move(name); }
		void set_arguments(ArgumentList* arguments) { m_arguments = arguments; }
		void set_statement(Statement* statement) { m_statement = statement; }

		// Geerbt über ASTNode
		virtual std::string to_string() const override;
		virtual void accept(ASTVisitor& visitor) override;
	};

	class FunctionDefinitionList : public ASTNode
	{
	public:
		std::vector<FunctionDefinition*> m_functions;
		void add_function(FunctionDefinition* func);
		Scope m_scope;
		// Geerbt über ASTNode
		virtual std::string to_string() const override;
		virtual void accept(ASTVisitor& visitor) override;
	};

	class Block : public StatementList
	{
	public:
		Block() 
			:m_statements{nullptr} 
		{}

		Scope m_scope;
		StatementList* m_statements;
		virtual std::string to_string() const override;
		virtual void accept(ASTVisitor& visitor) override;
	};

	class ParameterList : public ASTNode
	{
	public:
		std::vector<Expression*> m_parameters;
		void add_parameter(Expression* e);

		// Geerbt über ASTNode
		virtual std::string to_string() const override;
		virtual void accept(ASTVisitor& visitor) override;
	};

	class FunctionCallExpression : public Expression
	{
	public:
		std::string m_function_name;
		ParameterList* m_parameters;
		std::string to_string() const override;
		FunctionCallExpression() {}
	};

	//inline std::ostream& operator<<(std::ostream& out, const Expression* node)
	//{
	//	out << node->to_string();
	//	return out;
	//}

	inline std::ostream& operator<<(std::ostream& out, const ASTNode* node)
	{
		if (!node)
		{
			return out;
		}

		out << node->to_string();
		return out;
	}
}
