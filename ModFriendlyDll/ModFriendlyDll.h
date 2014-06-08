#ifndef MODFRIENDLYDLL_H
#define MODFRIENDLYDLL_H

#include "stdafx.h"

#ifdef MODFRIENDLYDLL_EXPORTS
#define MODFRIENDLYDLL_API __declspec(dllexport)
#else
#define MODFRIENDLYDLL_API __declspec(dllimport)
#endif

namespace ModFriendlyLib{
	
	MODFRIENDLYDLL_API bool simpleCheck();
	MODFRIENDLYDLL_API int simpleCalculation(int a, int b);
	MODFRIENDLYDLL_API void simpleOutput();

	class MODFRIENDLYDLL_API SimpleClass{
	public:
		SimpleClass();
		~SimpleClass();
		int functionMember(int a);
		int intMember;
	};

}

#endif