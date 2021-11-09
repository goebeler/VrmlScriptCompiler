#include <iostream>
#include <vector>

namespace stackmachine
{

	class stack;

	class operand
	{
	public:
		virtual ~operand() = default;
		virtual void eval(stack& current_stack)
		{
			throw std::runtime_error("Trying to eval an invalid operand.");
		}
	};

	class opcode_operand
	{
	public:
		explicit opcode_operand(/*const op_code code*/)
			/*:operation(code)*/
		{}
		virtual void execute(stack& current_stack) = 0;
		/*op_code operation;*/
	};

	class value_operand : public operand
	{
	public:
		value_operand() = default;
	};


	class program
	{
	public:
		program()
		{
			m_program_memory.resize(100);
		}
		
		void push(opcode_operand* op)
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
				throw std::runtime_error("Can not set instruction pointer. IP's adress exceeds program memory.");
			m_instruction_pointer = ip;
		}

		opcode_operand* pop()
		{
			if(m_instruction_pointer <= 0)
				return nullptr;
			return m_program_memory[--m_instruction_pointer];
		}

		/*opcode_operand* top()
		{
			return m_program_memory[m_instruction_pointer-1];
		}*/

		[[nodiscard]]bool program_is_empty() const
		{
			return m_instruction_pointer < 1;
		}

	private:
		unsigned m_instruction_pointer{};
		std::vector<opcode_operand*> m_program_memory;
	};

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

		bool is_empty() const
		{
			return m_stack_pointer < 1;
		}

	private:
		unsigned m_stack_pointer{};
		std::vector<operand*> m_stack;
	};


	class load_operand : public opcode_operand
	{
	public:
		explicit load_operand(value_operand* value_op)
			:m_value_op(value_op)
		{
			
		}

		void execute(stack& current_stack) override
		{
			current_stack.push(m_value_op);
		}

	private:
		value_operand* m_value_op;
	};
	
	class int_operand : public value_operand
	{
	public:
		explicit int_operand(const int value)
			: m_value(value)
		{}

		~int_operand() = default;
		
		int m_value;
	};
	
	class add_operand : public opcode_operand
	{
	public:
		explicit add_operand()
			: opcode_operand(/*op_code::add*/)
		{
		}

		void execute(stack& current_stack) override
		{
			//das ist doof
			auto* right = static_cast<int_operand*>(current_stack.pop());
			auto* left = static_cast<int_operand*>(current_stack.pop());

			current_stack.push(new int_operand(right->m_value + left->m_value));
		}
	};
	
	class stack_machine
	{
	public:
		stack_machine()
		= default;

		void run()
		{
			auto* op = m_code.pop();
			
			do
			{
				op->execute(m_stack);
				op = m_code.pop();
			}
			while ( op );

			return;
		}

		void load (opcode_operand* op)
		{
			m_code.push(op);
		}

		operand* get_stack_top()
		{
			return m_stack.top();
		}

	private:
		stack m_stack;
		program m_code;
		
	};
}
