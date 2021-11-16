#ifndef OPERAND_H
#define OPERAND_H

#include <stack.h>
#include <stdexcept>

namespace stackmachine
{
    class operand
	{
	public:
		virtual ~operand() = default;
		virtual void eval(stack& current_stack)
		{
			throw std::runtime_error("Trying to eval an invalid operand.");
		}
	};
}
#endif