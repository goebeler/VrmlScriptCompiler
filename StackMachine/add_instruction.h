#ifndef ADD_INSTRUCTION_H
#define ADD_INSTRUCTION_H

#include <instruction.h>
#include <int_operand.h>
#include <vrml_variant_operand.h>

namespace stackmachine
{
	template<typename ... Ts>
	struct overload : Ts ...
	{
		using Ts::operator() ...;
	};

    class add_instruction : public instruction
	{
	public:
		add_instruction()
		{
		}

		void execute(stack& current_stack) override
		{
			constexpr auto add_it = overload
			{
				[](sffloat lhs, sffloat rhs)->vrml_variant{ return vrml_variant{lhs + rhs};},
				[](sfint32 lhs, sfint32 rhs)->vrml_variant{ return vrml_variant{2*(lhs + rhs)};}, //wrong on purpose to see which lambda was called
				[](auto lhs, auto rhs)->vrml_variant{ return vrml_variant{std::string{"unsupported operation"}};}
			};

			//das ist doof
			//auto* right = static_cast<int_operand*>(current_stack.pop());
			//auto* left = static_cast<int_operand*>(current_stack.pop());

			auto* right = static_cast<vrml_variant_operand*>(current_stack.pop());
			auto* left = static_cast<vrml_variant_operand*>(current_stack.pop());

			current_stack.push(new vrml_variant_operand(std::visit(add_it,left->m_value, right->m_value)));
		}
	};
}

#endif