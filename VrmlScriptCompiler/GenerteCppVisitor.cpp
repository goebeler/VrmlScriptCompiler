#include "GenerateCppVisitor.h"
#include "fmt/format.h"

void vrmlast::GenerateCppVisitor::indent()
{
	m_indent += 4;
}

void vrmlast::GenerateCppVisitor::unindent()
{
	m_indent -= 4;
}

vrmlast::GenerateCppVisitor::GenerateCppVisitor()
{

}

void vrmlast::GenerateCppVisitor::print(std::string message)
{
	m_out << '\n' << std::string(m_indent, '-') << message;
}

void vrmlast::GenerateCppVisitor::visit(ArgumentList* args)
{
	if (!args)
		return;
	print(fmt::format("ArgumentList(size = {})", args->m_arguments.size()));
	//print("ArgumentList(size = " + args->m_arguments.size() + ")");
}

void vrmlast::GenerateCppVisitor::visit(FunctionDefinition* func)
{
	if (!func)
		return;

	print(fmt::format("void {0} ({1}){{2}}"
		, func->m_name
		, fmt::join(
			begin(func->m_arguments->m_arguments)
			, end(func->m_arguments->m_arguments)
			, ", ")
		,"//foo"));
}

void vrmlast::GenerateCppVisitor::visit(StatementList* statements)
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

void vrmlast::GenerateCppVisitor::visit(Expression* s)
{
	if (!s)
		return;

	m_out << "unknown expression";
}

void vrmlast::GenerateCppVisitor::visit(FunctionDefinitionList* s)
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

void vrmlast::GenerateCppVisitor::visit(Script* s)
{
	if (!s)
		return;

	m_out << '\n' << std::string(m_indent, '-') << "Script()";
	indent();
	s->m_functions->accept(*this);
	unindent();
}

void vrmlast::GenerateCppVisitor::visit(BinaryArithmeticExpression* s)
{
	if (!s) return;

	m_out << '\n' << std::string(m_indent, '-') << "BinaryArithmeticExpression()";
	indent();
	s->m_lhs->accept(*this);
	s->m_rhs->accept(*this);
	unindent();
}

void vrmlast::GenerateCppVisitor::visit(ParameterList* params)
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

void vrmlast::GenerateCppVisitor::visit(FunctionCallExpression* call)
{
	if (!call) return;

	m_out << '\n' << std::string(m_indent, '-') << "FunctionCallExpression(callee = " << call->m_function_name << ")";
	indent();
	call->m_parameters->accept(*this);
	unindent();
}

void vrmlast::GenerateCppVisitor::visit(Statement* statement)
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

void vrmlast::GenerateCppVisitor::visit(AssignmentExpression* a)
{
	if (!a) return;

	m_out << '\n' << std::string(m_indent, '-') << "AssignmentExpression()";
	indent();
	a->m_lhs->accept(*this);
	a->m_rhs->accept(*this);
	unindent();
}

void vrmlast::GenerateCppVisitor::visit(VariableExpression* a)
{
	if (!a) return;

	m_out << '\n' << std::string(m_indent, '-') << "VariableExpression( name = " << a->m_name << ")";
}

void vrmlast::GenerateCppVisitor::visit(IntConstantExpression* a)
{
	if (!a) return;

	m_out << '\n' << std::string(m_indent, '-') << "IntConstantExpression( value = " << a->m_value << ")";
}
