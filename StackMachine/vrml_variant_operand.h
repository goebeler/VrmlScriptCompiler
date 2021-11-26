#ifndef VRML_VARIANT_H
#define VRML_VARIANT_H

#include <string>
#include <variant>
#include <stack.h>
#include <value_operand.h>
#include <vrml_variable.h>

namespace stackmachine
{
	/*class sfvec3f : vrml_variable
	{
	public:
		
	private:
		float m_values[3]{};
	};

	class sfint32 : vrml_variable
	{
	public:
		sfint32(int i)
			:m_value(i)
		{}

	private:
		int m_value;
	};

	class sfstring
	{
	public:
		sfstring(const std::string value)
			: m_value(value)
		{}

		[[nodiscard]] std::string to_string() const
		{
			return m_value;
		}

	private:

		std::string m_value;

	};*/

	class sfnode: vrml_variable {};
	class mfnode: vrml_variable {};
	
	class sfint32: vrml_variable {};
	class sftime: vrml_variable {};
	class sfbool: vrml_variable {};
	class sffloat: vrml_variable {};
	class sfstring: vrml_variable {};
	class sfvec3f: vrml_variable {};
	class sfvec2f: vrml_variable {};
	class sfrotation: vrml_variable {};

	class mfint32: vrml_variable {};
	class mftime: vrml_variable {};
	class mfbool: vrml_variable {};
	class mffloat: vrml_variable {};
	class mfstring: vrml_variable {};
	class mfvec3f: vrml_variable {};
	class mfvec2f: vrml_variable {};
	class mfrotation: vrml_variable {};

	using vrml_variant = std::variant<std::monostate, sfint32, sffloat, sfstring>;

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