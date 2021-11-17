#ifndef STACK_FRAME_H
#define STACK_FRAME_H

namespace stack_machine
{
	
	class stack_frame
	{
	public:

	};

	class call_stack
	{
	public:
		void push(stack_frame* frame);
		stack_frame* pop();
	};

}

#endif