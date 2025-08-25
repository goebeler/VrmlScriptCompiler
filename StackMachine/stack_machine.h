#ifndef STACK_MACHINE_H
#define STACK_MACHINE_H

#include <instruction.h>
#include <iostream>
#include <map>
#include <operand.h>
#include <program.h>
#include <stack.h>
#include <vector>

namespace stackmachine {
class stack_machine {
public:
  stack_machine() = default;

  void run() {
    auto *instruction = m_code.pop();

    do {
      instruction->execute(m_stack);
      instruction = m_code.pop();
    } while (instruction);

    return;
  }

  void load(instruction *op) { m_code.append(op); }

  vrml_variable *get_stack_top() { return m_stack.top(); }

  void start_function(const std::string &name) {
    m_function_starts_by_name.emplace(name, m_code.get_instruction_pointer());
  }

  void end_function() {
    if (const instruction *current_instruction = m_code.peek();
        current_instruction != nullptr &&
        current_instruction->get_type() == instruction_type.RETURN)
  }

private:
  stack m_stack;
  program m_code;
  std::map<std::string, unsigned int> m_function_starts_by_name;
};
} // namespace stackmachine
#endif