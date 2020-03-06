#include "AssignVisitor.h"

void vrmlast::AssignVisitor::visit(VariableExpression* a)
{
	a->m_value = m_new_value;
}

void vrmlast::AssignVisitor::visit(ArgumentList* args){}
void vrmlast::AssignVisitor::visit(FunctionDefinition* func){}
void vrmlast::AssignVisitor::visit(ParameterList* params){}
void vrmlast::AssignVisitor::visit(FunctionCallExpression* call){}
void vrmlast::AssignVisitor::visit(Statement* statement){}
void vrmlast::AssignVisitor::visit(AssignmentExpression* a){}
void vrmlast::AssignVisitor::visit(IntConstantExpression* a){}
void vrmlast::AssignVisitor::visit(Block* a){}
void vrmlast::AssignVisitor::visit(StatementList* s){}
void vrmlast::AssignVisitor::visit(Expression* s){}
void vrmlast::AssignVisitor::visit(Script* s){}
void vrmlast::AssignVisitor::visit(FunctionDefinitionList* s){}
void vrmlast::AssignVisitor::visit(BinaryArithmeticExpression* s){}
