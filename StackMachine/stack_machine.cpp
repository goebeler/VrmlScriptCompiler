#include <iostream>
#include <vector>

namespace stackmachine
{

	//enum op_code
	//{
	//	push,
	//	pop,
	//	add,
	//};

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

	class opcode_operand : public operand
	{
	public:
		explicit opcode_operand(/*const op_code code*/)
			/*:operation(code)*/
		{}

		/*op_code operation;*/
	};

	class value_operand : public operand
	{
	public:
		value_operand() = default;
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

		void eval(stack& current_stack) override
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

		void eval(stack& current_stack) override
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
				op->eval(m_stack);
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
		stack m_code;
		
	};
}

int main()
{
	stackmachine::stack_machine machine;

	machine.load(new stackmachine::add_operand());
	machine.load(new stackmachine::load_operand(new stackmachine::int_operand(17)));
	machine.load(new stackmachine::load_operand(new stackmachine::int_operand(52)));
	

	machine.run();	
	
	//auto* result = static_cast<stackmachine::int_operand*>(result);
	std::cout << "Result = " << static_cast<stackmachine::int_operand*>(machine.get_stack_top())->m_value;	
}