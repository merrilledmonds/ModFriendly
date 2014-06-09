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

	void createPipe(int tests = 3);
	void readPipe();

	void startTests();
	void launchClient();

private:
	bool pipeRunning;
	int testCount;
	int desiredTests;
	LPTSTR pipename;
	LPCWSTR clientloc;
	DWORD dwThreadID;
	std::vector<HANDLE> pipes;
	std::map<HANDLE, bool> connected;
};

#endif