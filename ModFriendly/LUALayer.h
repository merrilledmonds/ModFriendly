#ifndef LUALAYER_H
#define LUALAYER_H

#include "stdafx.h"
extern "C"{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

class LUALayer{
public:
	LUALayer();
	~LUALayer();

	void startTests();

	static int exposedFunction(lua_State* L);
	static int pureCPPFunction(lua_State* L); //pure except for the return 0, where 0 is
											  //the number of variables pushed onto the stack

	void printstack(lua_State* L);
};

#endif