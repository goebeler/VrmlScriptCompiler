#ifndef VRML_VARIABLE_H
#define VRML_VARIABLE_H

#include <stdexcept>
#include <value_operand.h>

namespace stackmachine
{

	class sfnode;
	class mfnode;	

	class sfint32;
	class sftime;
	class sfbool;
	class sffloat;
	class sfstring;
	class sfvec3f;
	class sfvec2f;
	class sfrotation;

	class mfint32;
	class mftime;
	class mfbool;
	class mffloat;
	class mfstring;
	class mfvec3f;
	class mfvec2f;
	class mfrotation;

	class undefined_type;

	class vrml_variable : public value_operand
	{
	public:

		virtual vrml_variable* operator+(const vrml_variable& right) = 0;
		virtual vrml_variable* operator+(const sffloat& right) = 0;
		virtual const std::string to_string() const;

		virtual ~vrml_variable() = 0;

		virtual operator sfnode() const;
		virtual operator mfnode() const;

		virtual operator sfstring() const;
		virtual operator sfint32() const;
		virtual operator sftime() const;
		virtual operator sffloat() const;		
		virtual operator sfbool() const;
		virtual operator sfvec2f() const;
		virtual operator sfvec3f() const;
		virtual operator sfrotation() const;

		virtual operator mfstring() const;
		virtual operator mfint32() const;
		virtual operator mftime() const;
		virtual operator mffloat() const;
		virtual operator mfbool() const;
		virtual operator mfvec2f() const;
		virtual operator mfvec3f() const;		
		virtual operator mfrotation() const;

	};

	class vrml_variable_base : public vrml_variable
	{
		// Inherited via vrml_variable
		virtual vrml_variable* operator+(const vrml_variable& right) override;
		virtual vrml_variable* operator+(const sffloat& right) override;
	};

	class undefined_type : public vrml_variable
	{
	private:
		undefined_type()=default;
		~undefined_type()=default;

	public:
		static undefined_type& instance()
		{
			static undefined_type INSTANCE;
			return INSTANCE;
		}

		// Inherited via vrml_variable
		virtual vrml_variable* operator+(const vrml_variable& right) override;
		virtual vrml_variable* operator+(const sffloat& right) override;
	};

	inline vrml_variable::~vrml_variable() {}
	/*inline vrml_variable& vrml_variable::operator+(const vrml_variable& right)
	{
		return undefined_type::instance();
	}

	inline vrml_variable& vrml_variable::operator+(const sffloat& right)
	{
		return undefined_type::instance();
	}*/
}
#endif