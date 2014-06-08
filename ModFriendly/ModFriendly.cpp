// ModFriendly.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "IPCLayer.h"
#include "ModFriendlyDll.h"

#ifdef _DEBUG

int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "Connecting to pipe..." << std::endl;

    // Open the named pipe
    // Most of these parameters aren't very relevant for pipes.
    HANDLE pipe = CreateFile(
        L"\\\\.\\pipe\\IPCpipe",
        GENERIC_READ, // only need read access
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
 
    if (pipe == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to connect to pipe." << std::endl;
        // look up error code here using GetLastError()
        system("pause");
        return 1;
    }
 
    std::cout << "Reading data from pipe..." << std::endl;
 
    // The read operation will block until there is data to read
    wchar_t buffer[128];
    DWORD numBytesRead = 0;
    BOOL result = ReadFile(
        pipe,
        buffer, // the data from the pipe will be put here
        127 * sizeof(wchar_t), // number of bytes allocated
        &numBytesRead, // this will store number of bytes actually read
        NULL // not using overlapped IO
    );
 
    if (result) {
        buffer[numBytesRead / sizeof(wchar_t)] = '\0'; // null terminate the string
        std::cout << "Number of bytes read: " << numBytesRead << std::endl;
        std::wcout << "Message: " << buffer << std::endl;
    } else {
        std::cout << "Failed to read data from the pipe." << std::endl;
    }
 
    // Close our pipe handle
    CloseHandle(pipe);
 
    std::cout << "Done." << std::endl;
 

}
#else _RELEASE
int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "MODFRIENDLY, (c)2014 Merrill Edmonds" << std::endl;
	std::cout << "The source for this program is released under the MIT License." << std::endl;
	std::cout << "http://www.github.com/merrilledmonds" << std::endl << std::endl;

	std::cout << "Starting ModFriendly Checks." << std::endl;

	std::cout << "    Starting Dynamic Library Checks..." << std::endl;
	try{
		//simpleCalculation(int a, int b) is a .dll function that returns the sum of a and b.
		int result = ModFriendlyLib::simpleCalculation(1,2);
		std::cout << "        Simple Calculation Using Library: 1+2=" << ModFriendlyLib::simpleCalculation(1,2);
		if(result == 3){
			std::cout << " (OK)" << std::endl;
		}else{
			std::cout << " (ERROR)" << std::endl;
		}

		//simpleCheck() is a .dll function that returns a boolean (true)
		if(ModFriendlyLib::simpleCheck()){
			std::cout << "        Simple Boolean Check Using Library: True. (OK)" << std::endl;
		}else{
			std::cout << "        Simple Boolean Check Using Library: False. (ERROR)" << std::endl;
		}
		
		//test() is a .dll function that prints a test line using std::cout
		try{
			ModFriendlyLib::simpleOutput();
		}catch(std::exception& e){
			std::cout << "        Simple Function Check Using Library: " << e.what() << ". (ERROR)" << std::endl;
		}catch(...){
			std::cout << "        Simple Function Check Using Library: Unknown Exception. (ERROR)" << std::endl;
		}

		//SimpleClass is a .dll class with one member function and one member variable
		ModFriendlyLib::SimpleClass classInstance = ModFriendlyLib::SimpleClass();
		std::cout << "        Simple Class Creation Using Library: intMember:" << classInstance.intMember << " (OK)" << std::endl;
		std::cout << "                                             funMember:" << classInstance.functionMember(1) << " (OK)" << std::endl;

	}catch(std::exception& e){
		std::cout << "        Error loading dynamic library: " << e.what() << std::endl;
	}catch(...){
		std::cout << "        Unknown error loading dynamic library." <<std::endl;
	}

	std::cout << std::endl;
	std::cout << "    Starting Pipe Checks..." << std::endl;
	
	IPCLayer ipc = IPCLayer();
	ipc.createPipe();
	
    system("pause");
    return 0;
}
#endif