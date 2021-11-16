#include <load_instruction.h>

namespace stackmachine
{

    stackmachine::load_instruction::load_instruction(value_operand * value_op)
        :m_value_op(value_op)
    {
    }

    void load_instruction::execute(stack & current_stack)
    {
        current_stack.push(m_value_op);
    }
}