#ifndef STACK_H
#define STACK_H

#include <vector>

namespace stackmachine
{
    class vrml_variable;

	//template<typename T>
    class stack
	{
	public:
		stack()
		{
			m_stack.resize(100);
		}
		
		void push(vrml_variable* op)
		{
			if(m_stack_pointer >= m_stack.size())
			{
				m_stack.resize(m_stack.size() * 2);
			}
			m_stack[m_stack_pointer++] = op;
		}

		vrml_variable* pop()
		{
			if(m_stack_pointer <= 0)
				return nullptr;
			return m_stack[--m_stack_pointer];
		}

		vrml_variable* top()
		{
			return m_stack[m_stack_pointer-1];
		}

		[[nodiscard]] bool is_empty() const
		{
			return m_stack_pointer < 1;
		}

	private:
		unsigned m_stack_pointer{};
		std::vector<vrml_variable*> m_stack;
	};
}

#endif