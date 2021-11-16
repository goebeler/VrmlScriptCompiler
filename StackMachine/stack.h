#ifndef STACK_H
#define STACK_H

#include <vector>

namespace stackmachine
{
    class operand;
    
    class stack
	{
	public:
		stack()
		{
			m_stack.resize(100);
		}
		
		void push(operand* op)
		{
			if(m_stack_pointer >= m_stack.size())
			{
				m_stack.resize(m_stack.size() * 2);
			}
			m_stack[m_stack_pointer++] = op;
		}

		operand* pop()
		{
			if(m_stack_pointer <= 0)
				return nullptr;
			return m_stack[--m_stack_pointer];
		}

		operand* top()
		{
			return m_stack[m_stack_pointer-1];
		}

		[[nodiscard]] bool is_empty() const
		{
			return m_stack_pointer < 1;
		}

	private:
		unsigned m_stack_pointer{};
		std::vector<operand*> m_stack;
	};
}

#endif