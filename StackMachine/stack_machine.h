#ifndef STACK_MACHINE_H
#define STACK_MACHINE_H

#include <iostream>
#include <vector>
#include <stack.h>
#include <operand.h>
#include <instruction.h>
#include <program.h>

namespace stackmachine
{
	class stack_machine
	{
	public:
		stack_machine()
		= default;

		void run()
		{
			auto* instruction = m_code.pop();
			
			do
			{
				instruction->execute(m_stack);
				instruction = m_code.pop();
			}
			while ( instruction );

			return;
		}

		void load (instruction* op)
		{
			m_code.append(op);
		}

		vrml_variable* get_stack_top()
		{
			return m_stack.top();
		}

	private:
		stack m_stack;
		program m_code;
		
	};
}
#endif