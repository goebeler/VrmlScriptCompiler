#pragma once
#include <utility>
#include <variant>

namespace vrmlscript
{
	class SFString
	{
	public:
		SFString(const std::string value)
			: m_value(value)
		{}

		[[nodiscard]] std::string to_string () const
		{
			return m_value;
		}

	private:

		std::string m_value;

	};

	typedef int SFInt32;
	typedef float SFFloat;

	using VrmlVariant = std::variant<std::monostate, SFInt32, SFString> ;

}
