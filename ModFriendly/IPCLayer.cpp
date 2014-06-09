#include "stdafx.h"
#include "IPCLayer.h"

IPCLayer::IPCLayer(){
	pipename = TEXT("\\\\.\\pipe\\IPCpipe");
	clientloc = L"..\\Debug\\ModFriendly.exe";
	run();
};
IPCLayer::~IPCLayer(){

};

void IPCLayer::run(){pipeRunning = true;};
void IPCLayer::stop(){pipeRunning = false;};

void IPCLayer::createPipe(int tests){
	testCount = 0;
	desiredTests = tests;

	while(pipeRunning){
		if(testCount++ < desiredTests){
			launchClient();
		}else{
			stop();
			continue;
		}

		HANDLE newPipe = CreateNamedPipe(
			pipename,
			PIPE_ACCESS_DUPLEX,
			PIPE_TYPE_MESSAGE |
			PIPE_READMODE_MESSAGE |
			PIPE_WAIT,
			PIPE_UNLIMITED_INSTANCES,
			BUFSIZE,
			BUFSIZE,
			0,
			NULL);

		if(newPipe == NULL || newPipe == INVALID_HANDLE_VALUE){
			//Pipe creation failed
			std::cout<<"        Unable to create pipe.\n";
		}
		
		std::cout<<"        Connecting to client...\n";
		connected[newPipe] = ConnectNamedPipe(newPipe, NULL) ? true: (GetLastError() == ERROR_PIPE_CONNECTED);

		if(connected[newPipe]){

			HANDLE pipeThread = CreateThread(
				NULL,
				0,
				InstanceThread,
				(LPVOID) newPipe,
				0,
				&dwThreadID);

			if(pipeThread == NULL){
				//Thread creation failed
				std::cout<<"        Failed to create thread for client.\n";
			}else{
				CloseHandle(pipeThread);
			}
			
		}else{
			//Client failed to connect
			std::cout<<"        Client failed to connect.\n";
			CloseHandle(newPipe);
		}

	}//while running

	for(auto threadit = connected.begin(); threadit != connected.end(); threadit++){
		//std::cout<<"Waiting for thread.";
		HANDLE threadH = threadit->first;
		WaitForSingleObject(threadH, INFINITE);
	};
	Sleep(100);
};

DWORD WINAPI InstanceThread(HANDLE pipe){
	std::cout<<"        Connected to client.\n";
	std::cout<<"        Sending data...\n";

	const wchar_t *data = L"Sample message";
	DWORD numBytesWritten = 0;
	bool result = WriteFile(
		pipe,
		data,
		wcslen(data)*sizeof(wchar_t),
		&numBytesWritten,
		NULL);

	if(result){
		//Sent
		std::cout<<"        "<<numBytesWritten<<" bytes sent to client.\n";
	}else{
		//Failed send
		std::cout<<"        Failed to send data to client.\n";
	}
	
	CloseHandle(pipe);
	//Done writing
	std::cout<<"        Pipe closed. (OK)\n";
	return 0;
};

void IPCLayer::readPipe(){
	std::cout << "        Connecting to pipe..." << std::endl;

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
        std::cout << "        Failed to connect to pipe." << std::endl;
        // look up error code here using GetLastError()
        system("pause");
        return;
    }
 
    std::cout << "        Reading data from pipe..." << std::endl;
 
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
        std::cout << "        Number of bytes read: " << numBytesRead << std::endl;
        std::wcout << "        Message: " << buffer << std::endl;
    } else {
        std::cout << "        Failed to read data from the pipe." << std::endl;
    }
 
    // Close our pipe handle
    CloseHandle(pipe);
 
    std::cout << "Done." << std::endl;
	system("pause");

};

void IPCLayer::startTests(){
	try{
		createPipe();

		std::cout << "        ............................(OK)" << std::endl;
	}catch(...){
	}
};

void IPCLayer::launchClient(){
	ShellExecute(NULL, NULL, clientloc, NULL, NULL, SW_HIDE);
};