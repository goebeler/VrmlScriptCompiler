#include "nodes.hh"
#include <gcroot.h>

using namespace System::Linq::Expressions;
using namespace System::Collections::Generic;
using NetExp = System::Linq::Expressions::Expression;


namespace vrmlast
{

	ref class ExpressionBuilderRef
	{
	private:
	public:
		ExpressionBuilderRef()
		{}
		
		NetExp^ build_variableExpression(VariableExpression* a)
		{
			return NetExp::Variable(System::Int32::typeid, gcnew System::String(a->m_name.c_str()));
		}

		List<System::Linq::Expressions::MethodCallExpression^>^ build_script(Script* s)
		{
			
		}
	};

	class BuildExpressionVisitorBridge : public ASTVisitor
	{
	private:
		gcroot<ExpressionBuilderRef ^> m_builder;

	public:
		BuildExpressionVisitorBridge()
			:m_builder(gcnew ExpressionBuilderRef())
		{

		}

		// Inherited via ASTVisitor
		virtual void visit(ArgumentList* args) override
		{
			
		}
		virtual void visit(FunctionDefinition* func) override
		{
		}
		virtual void visit(ParameterList* params) override
		{
		}
		virtual void visit(FunctionCallExpression* call) override
		{
		}
		virtual void visit(Statement* statement) override
		{
		}
		virtual void visit(AssignmentExpression* a) override
		{
		}
		
		virtual void visit(VariableExpression* a) override
		{
		}

		virtual void visit(IntConstantExpression* a) override
		{
		}
		virtual void visit(StatementList* s) override
		{
		}
		virtual void visit(Expression* s) override
		{
		}

		NetExp^ build_function_body(StatementList* statements)
		{
			auto retval = NetExp::Block()
		}

		NetExp^ buildFunction(FunctionDefinition* functionDefinition)
		{
			auto bodyExp = build_function_body(functionDefinition->m_statements);
			auto parameters = build_function_parameters(functionDefinition->m_arguments);
			auto functionExpression = NetExp::Lambda(
				bodyExp
				, gcnew System::String(functionDefinition->m_name.c_str())
				, parametersExp);
		}

		virtual void visit(Script* s) override
		{
			auto retval = gcnew System::Collections::Generic::List<NetExp^>(10);
			for (const auto& functionDefinition : s->m_functions->m_functions)
			{
				retval->Add(build_function(functionDefinition));
			}
		}
		virtual void visit(FunctionDefinitionList* s) override
		{
		}
		virtual void visit(BinaryArithmeticExpression* s) override
		{
		}
	};
}