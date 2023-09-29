#include <stack_machine.h>
#include <add_instruction.h>
#include <load_instruction.h>
#include <int_operand.h>
#include <fmt/core.h>

int main()
{
	stackmachine::stack_machine machine;

	//machine.load(new stackmachine::add_instruction());
	//machine.load(new stackmachine::load_instruction(new stackmachine::int_operand(17)));
	//machine.load(new stackmachine::load_instruction(new stackmachine::int_operand(52)));

	/*machine.load(new stackmachine::add_instruction());
	machine.load(new stackmachine::load_instruction(new stackmachine::vrml_variant_operand(1.0f)));
	machine.load(new stackmachine::add_instruction());
	machine.load(new stackmachine::load_instruction(new stackmachine::vrml_variant_operand(17.5f)));
	machine.load(new stackmachine::load_instruction(new stackmachine::vrml_variant_operand(52.0f)));*/

	//yes it's a memory leak!
	machine.load(new stackmachine::add_instruction());
	machine.load(new stackmachine::load_instruction(new stackmachine::sffloat(1.0f)));
	machine.load(new stackmachine::add_instruction());
	machine.load(new stackmachine::load_instruction(new stackmachine::sffloat(17.5f)));
	machine.load(new stackmachine::load_instruction(new stackmachine::sffloat(52.0f)));

	machine.run();	
	
	// works only with vrml_variable as variant
	/*constexpr auto printer = stackmachine::overload
			{
				[](stackmachine::sffloat value)-> std::string{ return fmt::format("{0}",value.value);},
				[](stackmachine::sfint32 value)-> std::string{ return fmt::format("{0}",value.value);},
				[](auto value)-> std::string{ return "unsupported operation";}
			};*/

	auto* result = machine.get_stack_top();

	// works only with vrml_variable as variant
	//std::cout << "Result = " << std::visit(printer, result->m_value);	
	
	std::cout << "Result = " << result->to_string();
}
