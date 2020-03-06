#pragma once
#include <variant>

namespace vrmlscript
{
	class SFString {};

	class SFInt32
	{
	public:
		SFInt32(int value) :m_value{ value } {}
		operator int() const { return m_value; }
		inline std::variant<std::monostate, SFInt32, SFString> operator+(const SFString& rhs);
	
		int m_value;
	};
	//VrmlVariant operator+(std::monostate lhs, std::monostate rhs) { return VrmlVariant{ std::monostate{} }; }
	
	using VrmlVariant = std::variant<std::monostate, SFInt32, SFString> ;
	
	VrmlVariant SFInt32::operator+(const SFString& rhs)
	{
		return VrmlVariant{ std::monostate{} };
	}
}
