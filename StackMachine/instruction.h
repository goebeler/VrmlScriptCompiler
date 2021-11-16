#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stack.h>

namespace stackmachine
{
    class instruction
	{
	public:
		virtual ~instruction() = 0;
		virtual void execute(stack& current_stack) = 0;
	};

	inline instruction::~instruction()
	{
		
	}
}

#endif