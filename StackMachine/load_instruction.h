#ifndef LOAD_INSTRUCTION_H
#define LOAD_INSTRUCTION_H

#include <stack.h>
#include <instruction.h>
#include <value_operand.h>

namespace stackmachine
{
    
    class load_instruction : public instruction
	{
	public:
		explicit load_instruction(vrml_variable* value_op);
			

		void execute(stack& current_stack) override;

	private:
		vrml_variable* m_value_op;
	};
}

#endif