#ifndef INT_OPERAND_H
#define INT_OPERAND_H

#include <value_operand.h>

namespace stackmachine
{
    class int_operand : public value_operand
	{
	public:
		explicit int_operand(const int value)
			: m_value(value)
		{}

		~int_operand();
		
		int m_value;
	};

	inline int_operand::~int_operand()
	{
		
	}

	
}
#endif