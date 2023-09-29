#include <vrml_variable.h>
#include <vrml_variant_operand.h>

namespace stackmachine
{
	vrml_variable::operator sfnode() const
	{
		throw std::runtime_error("Can not convert to SFNode");
	}

	vrml_variable::operator mfnode() const
	{
		throw std::runtime_error("Can not convert to MFNode");
	}

	vrml_variable::operator sfstring() const
	{
		throw std::runtime_error("Can not convert to SFString");
	}

	vrml_variable::operator sfint32() const
	{
		throw std::runtime_error("Can not convert to SFInt32");
	}

	vrml_variable::operator sftime() const
	{
		throw std::runtime_error("Can not convert to SFTime");
	}

	vrml_variable::operator sffloat() const
	{
		throw std::runtime_error("Can not convert to SFFloat");
	}

	vrml_variable::operator sfbool() const
	{
		throw std::runtime_error("Can not convert to SFBool");
	}

	vrml_variable::operator sfvec2f() const
	{
		throw std::runtime_error("Can not convert to SFVec2f");
	}

	vrml_variable::operator sfvec3f() const
	{
		throw std::runtime_error("Can not convert to SFVec3f");
	}

	vrml_variable::operator sfrotation() const
	{
		throw std::runtime_error("Can not convert to SFRotation");
	}

	vrml_variable::operator mfstring() const
	{
		throw std::runtime_error("Can not convert to MFString");
	}

	vrml_variable::operator mfint32() const
	{
		throw std::runtime_error("Can not convert to MFInt32");
	}

	vrml_variable::operator mftime() const
	{
		throw std::runtime_error("Can not convert to MFTime");
	}

	vrml_variable::operator mffloat() const
	{
		throw std::runtime_error("Can not convert to MFFloat");
	}

	vrml_variable::operator mfbool() const
	{
		throw std::runtime_error("Can not convert to MFBool");
	}

	vrml_variable::operator mfvec3f() const
	{
		throw std::runtime_error("Can not convert to MFVec3f");
	}

	vrml_variable::operator mfvec2f() const
	{
		throw std::runtime_error("Can not convert to MFVec2f");
	}

	vrml_variable::operator mfrotation() const
	{
		throw std::runtime_error("Can not convert to MFRotation");
	}

	vrml_variable* undefined_type::operator+(const vrml_variable& right)
	{
		return &undefined_type::instance();
	}
	vrml_variable* undefined_type::operator+(const sffloat& right)
	{
		return &undefined_type::instance();
	}
	vrml_variable* vrml_variable_base::operator+(const vrml_variable& right)
	{
		return nullptr;
	}
	vrml_variable* vrml_variable_base::operator+(const sffloat& right)
	{
		return nullptr;
	}

	const std::string vrml_variable::to_string() const
	{
		return "";
	}
}
