#include "PrintASTVisitor.hh"


void vrmlast::PrintASTVisitor::indent()
{
	m_indent += 4;
}

void vrmlast::PrintASTVisitor::unindent()
{
	m_indent -= 4;
}

vrmlast::PrintASTVisitor::PrintASTVisitor()
{
	
}

void vrmlast::PrintASTVisitor::visit(ArgumentList* args)
{
	if (!args)
		return;

	m_out << '\n' << std::string(m_indent, '-') << "ArgumentList(size = " << args->m_arguments.size() << ")";
}

void vrmlast::PrintASTVisitor::visit(FunctionDefinition* func)
{
	if (!func)
		return;

	m_out << '\n' << std::string(m_indent, '-') << "FunctionDefinition(name = " << func->m_name << ")";
	indent();
	func->m_arguments->accept(*this);
	func->m_statement->accept(*this);
	unindent();
}

void vrmlast::PrintASTVisitor::visit(StatementList* statements)
{
	if (!statements)
		return;

	m_out << '\n' << std::string(m_indent, '-') << "StatementList(size = " << statements->m_statements.size() << ")";
	indent();
	for (const auto& statement : statements->m_statements)
	{
		statement->accept(*this);
	}
	unindent();
}

void vrmlast::PrintASTVisitor::visit(Expression* s)
{
	if (!s)
		return;

	m_out << "unknown expression";
}

void vrmlast::PrintASTVisitor::visit(FunctionDefinitionList* s)
{
	if (!s)
		return;

	m_out << '\n' << std::string(m_indent, '-') << "FunctionDefinitionList(size = " << s->m_functions.size() << ")";

	indent();
	for (const auto& func : s->m_functions)
	{
		func->accept(*this);
	}
	unindent();
}

void vrmlast::PrintASTVisitor::visit(Script* s)
{
	if (!s)
		return;

	m_out << '\n' << std::string(m_indent, '-') << "Script()";
	indent();
	s->m_functions->accept(*this);
	unindent();
}

void vrmlast::PrintASTVisitor::visit(BinaryArithmeticExpression* s)
{
	if (!s) return;

	m_out << '\n' << std::string(m_indent, '-') << "BinaryArithmeticExpression()";
	indent();
	s->m_lhs->accept(*this);
	s->m_rhs->accept(*this);
	unindent();
}

void vrmlast::PrintASTVisitor::visit(ParameterList* params)
{
	if (!params)
		return;

	m_out << '\n' << std::string(m_indent, '-') << "ParameterList(size = " << params->m_parameters.size() << ")";

	indent();
	for (const auto& parameter : params->m_parameters)
	{
		parameter->accept(*this);
	}
	unindent();
}

void vrmlast::PrintASTVisitor::visit(FunctionCallExpression* call)
{
	if (!call) return;

	m_out << '\n' << std::string(m_indent, '-') << "FunctionCallExpression(callee = " << call->m_function_name << ")";
	indent();
	call->m_parameters->accept(*this);
	unindent();
}

void vrmlast::PrintASTVisitor::visit(Statement* statement)
{
	if (!statement) return;

	m_out << '\n' << std::string(m_indent, '-') << "Statement(num_expressions = " << statement->m_expressions.size() << ")";
	indent();
	for (const auto& exp : statement->m_expressions)
	{
		exp->accept(*this);
	}
	unindent();
}

void vrmlast::PrintASTVisitor::visit(AssignmentExpression* a)
{
	if (!a) return;

	m_out << '\n' << std::string(m_indent, '-') << "AssignmentExpression()";
	indent();
	a->m_lhs->accept(*this);
	a->m_rhs->accept(*this);
	unindent();
}

void vrmlast::PrintASTVisitor::visit(VariableExpression* a)
{
	if (!a) return;

	m_out << '\n' << std::string(m_indent, '-') << "VariableExpression( name = " << a->m_name << ")";
}

void vrmlast::PrintASTVisitor::visit(IntConstantExpression* a)
{
	if (!a) return;

	m_out << '\n' << std::string(m_indent, '-') << "IntConstantExpression( value = " << a->m_value << ")";
}
