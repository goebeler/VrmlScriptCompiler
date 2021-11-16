#ifndef VRML_VARIANT_H
#define VRML_VARIANT_H

#include <string>
#include <variant>
#include <stack.h>
#include <value_operand.h>

namespace stackmachine
{
    class sfstring
	{
	public:
		sfstring(const std::string value)
			: m_value(value)
		{}

		[[nodiscard]] std::string to_string () const
		{
			return m_value;
		}

	private:

		std::string m_value;

	};

	typedef int sfint32;
	typedef float sffloat;

	using vrml_variant = std::variant<std::monostate, sfint32, sffloat, sfstring> ;

	class vrml_variant_operand : public value_operand
	{
	public:
		explicit vrml_variant_operand(const vrml_variant& value)
			:m_value(value)
		{}

		void eval(stack& current_stack) override;
		~vrml_variant_operand() override;

		vrml_variant m_value;
	};

	inline vrml_variant_operand::~vrml_variant_operand()
	{
		
	}

}

#endif