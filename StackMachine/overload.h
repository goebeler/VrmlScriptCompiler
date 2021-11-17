#ifndef OVERLOAD_H
#define OVERLOAD_H

namespace stackmachine
{
    template<typename ... Ts>
	struct overload : Ts ...
	{
		using Ts::operator() ...;
	};
}

#endif