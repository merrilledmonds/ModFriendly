#include "stdafx.h"
#include "IPCLayer.h"

IPCLayer::IPCLayer(){
	pipename = TEXT("\\\\.\\pipe\\IPCpipe");
	pipeRunning = true;
};
IPCLayer::~IPCLayer(){

};

void IPCLayer::run(){pipeRunning = true;};
void IPCLayer::stop(){pipeRunning = false;};

void IPCLayer::createPipe(){
	while(pipeRunning){
	
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
			std::cout<<"Unable to create pipe.\n";
		}

		connected[newPipe] = ConnectNamedPipe(newPipe, NULL) ? true: (GetLastError() == ERROR_PIPE_CONNECTED);

		if(connected[newPipe]){
			std::cout<<"Connected to client...\n";

			HANDLE pipeThread = CreateThread(
				NULL,
				0,
				InstanceThread,
				(LPVOID) newPipe,
				0,
				&dwThreadID);

			if(pipeThread == NULL){
				//Thread creation failed
				std::cout<<"Failed to create thread for client.\n";
			}else{
				CloseHandle(pipeThread);
			}
			
		}else{
			//Client failed to connect
			std::cout<<"Client failed to connect.\n";
			CloseHandle(newPipe);
		}

	}//while running
};

DWORD WINAPI InstanceThread(HANDLE pipe){
	std::cout<<"Connected to client.\n";
	std::cout<<"Sending data...\n";

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
		std::cout<<numBytesWritten<<" bytes sent to client.\n";
	}else{
		//Failed send
		std::cout<<"Failed to send data to client.\n";
	}
	
	CloseHandle(pipe);
	//Done writing
	std::cout<<"Write successful. Pipe closed.";
	return 0;
};

void IPCLayer::readPipe(){


};