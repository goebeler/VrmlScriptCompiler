#include "stack_machine.hpp"

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