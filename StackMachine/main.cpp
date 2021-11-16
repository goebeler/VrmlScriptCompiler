#include <stack_machine.h>
#include <add_instruction.h>
#include <load_instruction.h>

#include "int_operand.h"

int main()
{
	stackmachine::stack_machine machine;

	//machine.load(new stackmachine::add_instruction());
	//machine.load(new stackmachine::load_instruction(new stackmachine::int_operand(17)));
	//machine.load(new stackmachine::load_instruction(new stackmachine::int_operand(52)));

	machine.load(new stackmachine::add_instruction());
	machine.load(new stackmachine::load_instruction(new stackmachine::vrml_variant_operand(17.5f)));
	machine.load(new stackmachine::load_instruction(new stackmachine::vrml_variant_operand(52)));
	

	machine.run();	
	
	//auto* result = static_cast<stackmachine::int_operand*>(result);
	auto* result = static_cast<stackmachine::vrml_variant_operand*>(machine.get_stack_top());
	std::cout << "Result = " << std::get<stackmachine::sfstring>(result->m_value).to_string();	
}
