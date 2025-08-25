#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "stack.h"

namespace stackmachine {
enum class instruction_type { RETURN, LOAD, ADD };

class instruction {
public:
  virtual ~instruction() = 0;
  virtual void execute(stack &current_stack) = 0;
  virtual instruction_type get_type() = 0;
};

inline instruction::~instruction() {}
} // namespace stackmachine

#endif