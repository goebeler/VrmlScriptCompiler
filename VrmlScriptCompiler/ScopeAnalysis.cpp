#include "ScopeAnalysis.h"

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

}

void vrmlast::ScopeAnalysis::visit(AssignmentExpression* a)
{
}

void vrmlast::ScopeAnalysis::visit(VariableExpression* a)
{
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
}
