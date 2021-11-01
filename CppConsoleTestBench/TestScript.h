#pragma once
#include "../VrmlScriptCompiler/VrmlVariant.h"
#include <iostream>
using namespace vrmlscript;

namespace scripting_test
{
	class TestScript
	{
	public:
		SFString stringField;
		SFInt32 intField;

		TestScript()
			: stringField(""), intField(0)
		{}

		void initialize()
		{
			std::cout << __FILE__ << ":" << __LINE__ << "In initialize";
			std::cout << "stringField = " << stringField.to_string();
			std::cout << "intField = " << intField;
		}

	private:
	};
}