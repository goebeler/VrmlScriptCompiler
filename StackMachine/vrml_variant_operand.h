#ifndef VRML_VARIANT_H
#define VRML_VARIANT_H

#include <string>
#include <variant>
#include <stack.h>
#include <value_operand.h>
#include <vrml_variable.h>
#include <fmt/core.h>

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

	class sfnode: vrml_variable_base {};
	class mfnode: vrml_variable_base {};
	
	class sfint32: public vrml_variable_base
	{
	public: 
		int32_t value;
		const std::string to_string() const override
		{
			return fmt::format("{} (int)", value);
		}
	};

	class sftime: vrml_variable_base {};
	class sfbool: vrml_variable_base {};

	class sffloat: public vrml_variable_base
	{
	public:
		sffloat(const float f):value(f){}
		float value;

		sffloat* operator+(const vrml_variable& right) override
		{
			auto retval = new sffloat(value + ((sffloat&)right).value);
			return retval;
		}

		sffloat* operator+(const sffloat& right) override
		{
			auto retval = new sffloat(value + ((sffloat&)right).value);
			return retval;
		}

		const std::string to_string() const override
		{
			return fmt::format("{} (float)", value);
		}
	};
	class sfstring: vrml_variable_base {};
	class sfvec3f: vrml_variable_base {};
	class sfvec2f: vrml_variable_base {};
	class sfrotation: vrml_variable_base {};

	class mfint32: vrml_variable_base {};
	class mftime: vrml_variable_base {};
	class mfbool: vrml_variable_base {};
	class mffloat: vrml_variable_base {};
	class mfstring: vrml_variable_base {};
	class mfvec3f: vrml_variable_base {};
	class mfvec2f: vrml_variable_base {};
	class mfrotation: vrml_variable_base {};

	using vrml_variant = std::variant<std::monostate, sfint32, sffloat, sfstring>;

	/*class vrml_variant_operand : public value_operand
	{
	public:
		explicit vrml_variant_operand(const vrml_variant& value)
			:m_value(value)
		{}

		void eval(stack& current_stack) override;
		~vrml_variant_operand() override;

		vrml_variant m_value;
	};*/

	/*inline vrml_variant_operand::~vrml_variant_operand()
	{

	}*/

}

#endif