#ifndef VALUE_OPERAND_H
#define VALUE_OPERAND_H

#include <operand.h>

namespace stackmachine
{
    class value_operand : public operand
	{
	public:
		value_operand() = default;
	};

}
#endif