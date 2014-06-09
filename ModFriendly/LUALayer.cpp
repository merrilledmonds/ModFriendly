#include "stdafx.h"
#include "LUALayer.h"

LUALayer::LUALayer(){

};
LUALayer::~LUALayer(){
};

void LUALayer::startTests(){

	std::cout << "        Create new Lua State from C++: ";
	lua_State* s = lua_open();
	if(s){
		std::cout << "(OK)" << std::endl;

		luaL_openlibs(s);

		luaL_dostring(s, "a=12");
		lua_getglobal(s, "a");
		int globalresult = lua_tointeger(s, -1);
		std::cout << "        Set/Get Global Variable Using Lua from C++: ";
		if(globalresult == 12){
			std::cout << " Success. (OK)" << std::endl;
		}else{
			std::cout << " Failed. (ERROR)" <<std::endl;
		}


		std::cout << "        Calling a Lua String from C++: ";
		if(luaL_dostring(s, "print(\"Success. (OK)\")")){
			std::cout << "Failed. (ERROR)" << std::endl;
		}


		std::cout << "        Calling a Lua Script from C++: Loaded. ";
		int error = luaL_dofile(s,"../ModFriendly/lua/luatest.lua");
		if(!error){
			std::cout << "(OK)" << std::endl;
			
			std::cout << "                                       pcall. ";
			error = lua_pcall(s, 0, LUA_MULTRET, 0);
			if(!error){
				std::cout << error << "(ERROR)" << std::endl;
			}else{
				std::cout << "(OK)" << std::endl;
				
				std::cout << "                                       Global variable read. ";
				lua_getglobal(s,"somenumber");
				lua_getglobal(s,"someothernumber");
				int somenumber = (int)lua_tonumber(s, -2);
				int someothernumber = (int)lua_tonumber(s, -1);
				lua_pop(s,1); lua_pop(s,1);
				if(somenumber == 100 && someothernumber == 200){
					std::cout << "(OK)" << std::endl;
				}else{
					std::cout << "(ERROR)" << std::endl;
				}

				std::cout << "                                       Global variable write. ";
				lua_pushnumber(s, 300);
				lua_setglobal(s, "somenumber");
				lua_pop(s,1);
				lua_getglobal(s, "somenumber");
				somenumber = (int)lua_tonumber(s, -1);
				lua_pop(s,1);
				if(somenumber == 300){
					std::cout << "(OK)" << std::endl;
				}else{
					std::cout << "(ERROR)" << std::endl;
				}

				std::cout << "                                       Function call. ";
				lua_getglobal(s,"sum");
				lua_pushnumber(s,1);
				lua_pushnumber(s,2);
				if(lua_pcall(s,2,1,0)){
					std::cout << "(ERROR)" << std::endl;
				}else{
					std::cout << "(OK)" << std::endl;
				}

				std::cout << "                                       Return variable from Lua. ";
				int sumresult = (int)lua_tonumber(s,-1);
				lua_pop(s,1);
				if(sumresult == 3){
					std::cout << "(OK)" << std::endl;
				}else{
					std::cout << "(ERROR)" << std::endl;
				}
				

				std::cout << "        Calling a C++ function from Lua: Register function. ";
				lua_register(s,"CPPFunction",exposedFunction);
				lua_getglobal(s,"CPPFunction");
				if(lua_iscfunction(s,1)){
					std::cout << "(OK)" << std::endl;
				}else{
					std::cout << "(ERROR)" << std::endl;
				}
				lua_pop(s,1);

				std::cout << "                                         Call a pure C++ function. ";
				lua_register(s,"PureCPPFunction",pureCPPFunction);
				lua_getglobal(s,"callPureCPP");
				if(lua_pcall(s,0,0,0)){
					std::cout << "(ERROR)" << std::endl;
				}
				std::cout << "                                         Call a C++/Lua fn. ";
				lua_getglobal(s,"callCPP");
				if(lua_pcall(s,0,0,0)){ //callCPP prints "OK"
					std::cout << "(ERROR)" << std::endl; //if callCPP fails
				}
				lua_pop(s,-1); //get rid of callCPP
				std::cout << "        Stack Size Check: ";
				if(lua_gettop(s)==0){
					std::cout << "Stack empty. (OK)" << std::endl;
				}else{
					std::cout << "Stack not empty. (ERROR)" << std::endl;
				}
				while(lua_gettop(s)){ lua_pop(s,1); }
				std::cout << "        Stack Push Check: ";
				lua_pushnumber(s,100);
				if(lua_gettop(s)==1){
					std::cout << "Success. (OK)" << std::endl;
				}else{
					std::cout << "Failed. (ERROR)" << std::endl;
				}
				std::cout << "        Stack Pop Check: ";
				lua_pop(s,1);
				if(lua_gettop(s)==0){
					std::cout << "Success. (OK)" << std::endl;
				}else{
					std::cout << "Failed. (ERROR)" << std::endl;
				}

			}
		
		}else{
			std::cout << error << "(ERROR)" << std::endl;
		}

		lua_close(s);
	
	}else{
		std::cout << "(ERROR)" << std::endl;
	}
};

int LUALayer::pureCPPFunction(lua_State* L){
	std::cout << "(OK)" << std::endl;
	return 0;
}

int LUALayer::exposedFunction(lua_State* L){
	int stackheight = lua_gettop(L);
	//clear stack
	while(stackheight>0){
		lua_pop(L,1);
	}

	lua_pushstring(L,"(OK)");

	return 1;
};

void LUALayer::printstack(lua_State* L){
	int top = lua_gettop(L);

	for(int i=1; i<top; i++){
		int t = lua_type(L,i);
		switch(t){
		case LUA_TSTRING:
			std::cout << lua_tostring(L,i);
			break;
		case LUA_TBOOLEAN:
			std::cout << lua_toboolean(L,i);
			break;
		case LUA_TNUMBER:
			std::cout << lua_tonumber(L,i);
			break;
		default:
			std::cout << lua_typename(L,i);
			break;
		}
		std::cout << " ";
	}
};