#pragma once
#include "VrmlVariant.h"

namespace vrmlscript
{
	class ScriptBase
	{
	public:
		virtual ~ScriptBase() = 0;
		static VrmlVariant ArithAdd(VrmlVariant lhs, VrmlVariant rhs)
		{
			if (auto lhsIntVal = std::get_if<SFInt32>(&lhs)
				, rhsIntVal = std::get_if<SFInt32>(&rhs);
				lhsIntVal && rhsIntVal)
			{
				return VrmlVariant{ *lhsIntVal + *rhsIntVal};
			}
			else 
			{
				return std::monostate{};
			}
		}
	};

	inline ScriptBase::~ScriptBase() {};
}
