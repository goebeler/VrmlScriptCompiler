#ifndef ADD_INSTRUCTION_H
#define ADD_INSTRUCTION_H

#include <instruction.h>
#include <int_operand.h>
#include <vrml_variant_operand.h>
#include <overload.h>

namespace stackmachine
{
	class return_instrucion : public instruction
	{
		// Geerbt über instruction
		virtual void execute(stack& current_stack) override;
		virtual instruction_type get_type() override;
	};

	class add_instruction : public instruction
	{
	public:
		add_instruction()
		{
		}

		void execute(stack& current_stack) override
		{
			//constexpr auto add_it = overload
			//{
			//	[](sffloat lhs, sffloat rhs)->vrml_variant{ return vrml_variant{lhs + rhs};},
			//	[](sfint32 lhs, sfint32 rhs)->vrml_variant{ return vrml_variant{2*(lhs + rhs)};}, //wrong on purpose to see which lambda was called
			//	[](auto lhs, auto rhs)->vrml_variant{ return vrml_variant{std::string{"unsupported operation"}};}
			//};

			//das ist doof
			//auto* right = static_cast<int_operand*>(current_stack.pop());
			//auto* left = static_cast<int_operand*>(current_stack.pop());


			//works:
			//auto* right = static_cast<vrml_variant_operand*>(current_stack.pop());
			//auto* left = static_cast<vrml_variant_operand*>(current_stack.pop());

			/*const auto* right = static_cast<vrml_variable*>(current_stack.pop());
			auto* left = static_cast<vrml_variable*>(current_stack.pop());

			auto& retval = left->operator+(*right);
			current_stack.push(&retval);*/

			auto right = current_stack.pop();
			auto left = current_stack.pop();
			current_stack.push(left->operator+(*right));

			//current_stack.push(new vrml_variant_operand(std::visit(add_it,left->m_value, right->m_value)));
			//current_stack.push(new vrml_variant_operand{left + right});

		}

		instruction_type get_type() override
		{
			return instruction_type::ADD;
		}
	};
}

#endif