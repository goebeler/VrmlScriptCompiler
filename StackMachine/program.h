#ifndef PROGRAM_H
#define PROGRAM_H

#include <stdexcept>
#include <vector>

namespace stackmachine
{
    class instruction;

    class program
	{
	public:
		program()
		{
			m_program_memory.resize(100);
		}
		
		void append(instruction* op)
		{
			if(m_instruction_pointer >= m_program_memory.size())
			{
				m_program_memory.resize(m_program_memory.size() * 2);
			}
			m_program_memory[m_instruction_pointer++] = op;
		}

		[[nodiscard]] unsigned int get_instruction_pointer() const
		{
			return m_instruction_pointer;
		}

		void set_instruction_pointer(unsigned int ip)
		{
			if(ip < 0 || ip >= m_program_memory.size())
				throw std::runtime_error("Can not set instruction pointer. IP's address exceeds program memory.");
			m_instruction_pointer = ip;
		}

		instruction* pop()
		{
			if(m_instruction_pointer <= 0)
				return nullptr;
			return m_program_memory[--m_instruction_pointer];
		}

		/*instruction* top()
		{
			return m_program_memory[m_instruction_pointer-1];
		}*/

		[[nodiscard]]bool program_is_empty() const
		{
			return m_instruction_pointer < 1;
		}

	private:
		unsigned m_instruction_pointer{};
		std::vector<instruction*> m_program_memory;
	};

}
#endif