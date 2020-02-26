#pragma once
#include <vector>
#include <string>
#include <memory>
#include <iostream>

namespace vrmlast
{
	class ArgumentList;
	class FunctionDefinition;
	class ParameterList;
	class FunctionCallExpression;
	class Statement;	
	class AssignmentExpression;
	class VariableExpression;
	class IntConstantExpression;
	class IntConstantExpression;

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
	};

	class ASTNode
	{
	public:
		virtual ~ASTNode() {};
		virtual std::string to_string() const = 0;
		virtual void accept(ASTVisitor& visitor) = 0;
	};

	inline std::ostream& operator<<(std::ostream& out, const ASTNode& node)
	{
		out << node.to_string();
		return out;
	}
	
	class Expression : public ASTNode
	{
	public:
		std::string m_type;
		std::string m_value;

		// Geerbt über ASTNode
		virtual std::string to_string() const override;
		virtual void accept(ASTVisitor& visitor) override;
	};

	class VariableExpression : public Expression
	{
	public:
		std::string m_name;

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

		ArgumentList(){}
		
		void add_argument(std::string name);

		// Geerbt über ASTNode
		virtual std::string to_string() const override;
		virtual void accept(ASTVisitor& visitor) override;
	};

	class AssignmentExpression : public Expression
	{
	public:
		Expression m_lhs;
		Expression m_rhs;

		// Geerbt über ASTNode
		virtual std::string to_string() const override;
		virtual void accept(ASTVisitor& visitor) override;
	};
	
	class Statement : public ASTNode
	{
	public:
		std::vector<Expression> m_expressions;
		void add_expression(Expression exp);


		// Geerbt über ASTNode
		virtual std::string to_string() const override;
		virtual void accept(ASTVisitor& visitor) override;
	};
	
	class StatementList : public ASTNode
	{
	public:
		std::vector<Statement> m_statements;
		void add_statement(Statement statement);
		friend std::ostream& operator<<(std::ostream& out, const StatementList& statement_list);

		// Geerbt über ASTNode
		virtual std::string to_string() const override;
		virtual void accept(ASTVisitor& visitor) override;
	};

	class FunctionDefinition : public ASTNode
	{
	public:
		std::string m_name;
		ArgumentList m_arguments;
		StatementList m_statements;

		FunctionDefinition(){}
		void set_name(std::string name) { m_name = std::move(name); }
		void set_arguments(ArgumentList arguments) { m_arguments = std::move(arguments); }
		void set_statements(StatementList statements) { m_statements = std::move(statements); }

		// Geerbt über ASTNode
		virtual std::string to_string() const override;
		virtual void accept(ASTVisitor& visitor) override;
	};

	class FunctionDefinitionList : public ASTNode
	{
	public:
		std::vector<FunctionDefinition> m_functions;
		void add_function(FunctionDefinition func);

		// Geerbt über ASTNode
		virtual std::string to_string() const override;
		virtual void accept(ASTVisitor& visitor) override;
	};
	
	class Block {};	

	class ParameterList : public ASTNode
	{
	public:
		std::vector<Expression> m_parameters;
		void add_parameter(Expression e);
		
		// Geerbt über ASTNode
		virtual std::string to_string() const override;
		virtual void accept(ASTVisitor& visitor) override;
	};

	class FunctionCallExpression : public Expression
	{
	public:
		std::string to_string() const override;
		FunctionCallExpression(std::string functionName, ParameterList parameters) {}
	};
}
