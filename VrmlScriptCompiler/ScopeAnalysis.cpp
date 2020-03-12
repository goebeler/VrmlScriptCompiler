#include "ScopeAnalysis.h"
#include <string>

vrmlast::VariableExpression* vrmlast::ScopeAnalysis::find_variable(std::string name)
{
	auto scope = m_current_scope;
	vrmlast::VariableExpression* found_variable = nullptr;

	while (scope != nullptr && found_variable == nullptr)
	{
		const auto& found = scope->m_members.find(name);
		if (found != scope->m_members.end())
		{
			found_variable = found->second.get();
			break;
		}
		scope = scope->m_parent;
	}

	return found_variable;
}

void vrmlast::ScopeAnalysis::visit(ArgumentList* args)
{
}

void vrmlast::ScopeAnalysis::visit(FunctionDefinition* func)
{
	func->m_scope.m_parent = m_current_scope;
	m_current_scope = &func->m_scope;

	for (const auto& argument : func->m_arguments->m_arguments)
	{
		func->m_scope.m_members.emplace(
			std::make_pair(argument,std::make_unique<VariableExpression>(argument)));
	}

	func->m_statement->accept(*this);	
}

void vrmlast::ScopeAnalysis::visit(ParameterList* params)
{
}

void vrmlast::ScopeAnalysis::visit(FunctionCallExpression* call)
{
}

void vrmlast::ScopeAnalysis::visit(Statement* statement)
{
	for (const auto& exp : statement->m_expressions)
	{
		exp->accept(*this);
	}
}

void vrmlast::ScopeAnalysis::visit(AssignmentExpression* a)
{
	a->m_rhs->accept(*this);

	const auto& left = static_cast<VariableExpression*>(a->m_lhs);
	auto newscope = new Scope();
	newscope->m_parent = m_current_scope;
	m_current_scope = newscope;
	m_current_scope->m_members.emplace(
		std::make_pair(left->m_name, std::make_unique<VariableExpression>(left->m_name)));
	a->m_lhs = find_variable(left->m_name);

	
}

void vrmlast::ScopeAnalysis::visit(VariableExpression* a)
{
	auto found_variable = find_variable(a->m_name);
}

void vrmlast::ScopeAnalysis::visit(IntConstantExpression* a)
{
}

void vrmlast::ScopeAnalysis::visit(StatementList* s)
{
}

void vrmlast::ScopeAnalysis::visit(Expression* s)
{
}

void vrmlast::ScopeAnalysis::visit(Script* s)
{
	m_current_scope = &s->m_scope;

	auto functions = s->m_functions;
	for (const auto& function : functions->m_functions)
	{
		function->accept(*this);
	}
}

void vrmlast::ScopeAnalysis::visit(FunctionDefinitionList* s)
{
}

void vrmlast::ScopeAnalysis::visit(BinaryArithmeticExpression* s)
{
	s->m_rhs->accept(*this);
	s->m_lhs->accept(*this);
}

void vrmlast::ScopeAnalysis::visit(Block* s)
{
	for (const auto& statement : s->m_statements->m_statements)
	{
		statement->accept(*this);
	}
}
