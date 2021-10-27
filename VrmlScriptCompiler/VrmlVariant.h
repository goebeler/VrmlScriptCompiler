#pragma once
#include <variant>

namespace vrmlscript
{
	class SFString
	{
	public:
		SFString(const std::string value)
			: m_value(value)
		{}

		std::string to_string () const
		{
			return m_value;
		}

	private:

		std::string m_value;

	};

	typedef int32_t SFInt32;
	typedef float_t SFFloat;

	/*class SFInt32
	{
	public:
		SFInt32(int value) :m_value{ value } {}
		operator int() const { return m_value; }
		inline std::variant<std::monostate, SFInt32, SFString> operator+(const SFString& rhs);
	
		int m_value;
	};*/
	//VrmlVariant operator+(std::monostate lhs, std::monostate rhs) { return VrmlVariant{ std::monostate{} }; }
	
	using VrmlVariant = std::variant<std::monostate, SFInt32, SFString> ;

}
