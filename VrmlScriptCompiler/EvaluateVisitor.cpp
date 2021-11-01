#include "EvaluateVisitor.h"
#include <algorithm>
#include <cassert>

namespace vrmlast
{
	void evaluate_visitor::visit(ArgumentList* args)
	{
	}

	void evaluate_visitor::visit(FunctionDefinition* func)
	{
	}

	void evaluate_visitor::visit(ParameterList* params)
	{
	}

	void evaluate_visitor::visit(FunctionCallExpression* call)
	{
		m_current_stack_variables.clear();

		const auto function_definitions = m_script->m_functions->m_functions;
		//const auto call_expression = *call;
		auto has_function_name = [call](const FunctionDefinition* pFunction)->bool{return pFunction->m_name == call->m_function_name;};
		const auto it_function_defintion = std::ranges::find_if(function_definitions,has_function_name );

		if(it_function_defintion == function_definitions.end())
		{
			assert("Could not find function definition.");
		}

		const auto function_definition = *it_function_defintion;
		const auto parameter_names = function_definition->m_parameter_list->m_parameters;
		const auto call_arguments = call->m_argument_list->m_arguments;

		for (int i = 0; i < parameter_names.size(); ++i)
		{
			//call_arguments[i]->accept(*this);

			//function_definition->m_scope.m_members[parameter_names[i]] = 
		}
	}

	void evaluate_visitor::visit(Statement* statement)
	{
	}

	void evaluate_visitor::visit(AssignmentExpression* a)
	{
	}

	void evaluate_visitor::visit(VariableExpression* a)
	{
	}

	void evaluate_visitor::visit(IntConstantExpression* a)
	{
	}

	void evaluate_visitor::visit(StatementList* s)
	{
	}

	void evaluate_visitor::visit(Expression* s)
	{
	}

	void evaluate_visitor::visit(Script* s)
	{
		const auto function_definitions = s->m_functions->m_functions;
		constexpr auto is_initialize = [](const FunctionDefinition* pFunction){return pFunction->m_name == "initialize";};
		const auto it_initialize_function = std::ranges::find_if(function_definitions,is_initialize);

		if(it_initialize_function == function_definitions.end())
		{
			return;
		}
		auto initialize_call = new FunctionCallExpression();
		initialize_call->m_function_name = "initialize";
		
		visit(*it_initialize_function);
	}

	void evaluate_visitor::visit(FunctionDefinitionList* s)
	{
	}

	void evaluate_visitor::visit(Block* s)
	{
	}

	void evaluate_visitor::visit(BinaryArithmeticExpression* s)
	{
	}
}
