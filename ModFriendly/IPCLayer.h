#ifndef IPCLAYER_H
#define IPCLAYER_H

#include "stdafx.h"

#define BUFSIZE 512

DWORD WINAPI InstanceThread(HANDLE);

class IPCLayer{
public:
	IPCLayer();
	~IPCLayer();

	void run();
	void stop();

	void createPipe();
	void readPipe();

private:
	bool pipeRunning;
	LPTSTR pipename;
	DWORD dwThreadID;
	std::vector<HANDLE> pipes;
	std::map<HANDLE, bool> connected;
};

#endif