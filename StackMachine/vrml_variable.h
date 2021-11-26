#ifndef VRML_VARIABLE_H
#define VRML_VARIABLE_H

#include <stdexcept>

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

	class vrml_variable
	{
	public:
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

	inline vrml_variable::~vrml_variable() {}
}
#endif