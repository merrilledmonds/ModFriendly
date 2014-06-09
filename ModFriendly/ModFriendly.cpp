// ModFriendly.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "IPCLayer.h"
#include "DLLLayer.h"
#include "LUALayer.h"
#include "ScriptLayer.h"

#ifdef _DEBUG

int _tmain(int argc, _TCHAR* argv[])
{
	IPCLayer ipcTests = IPCLayer();
	ipcTests.readPipe();

}
#else _RELEASE
int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "MODFRIENDLY, (c)2014 Merrill Edmonds" << std::endl;
	std::cout << "The source for this program is released under the MIT License." << std::endl;
	std::cout << "http://www.github.com/merrilledmonds" << std::endl << std::endl;

	std::cout << "Starting ModFriendly Checks." << std::endl;

	std::cout << "    Starting Dynamic Library Checks..." << std::endl;
	
	DLLLayer dllTests = DLLLayer();
	dllTests.startTests();

	std::cout << std::endl;
	std::cout << "    Starting Pipe Checks..." << std::endl;
	
	IPCLayer ipcTests = IPCLayer();
	ipcTests.startTests();
	
	std::cout << std::endl;
	std::cout << "    Starting Lua Checks..." << std::endl;

	LUALayer luaTests = LUALayer();
	luaTests.startTests();

	std::cout << std::endl;
	std::cout << "    Starting Custom Script Checks..." << std::endl;

	ScriptLayer scriptTests = ScriptLayer();
	scriptTests.startTests();

    system("pause");
    return 0;
}
#endif