#ifndef API_H
#define API_H

#include "stdafx.h"

class API{
public:
	API();
	~API();

	void APIFUNC_1();
	int APIFUNC_2();
	bool APIFUNC_3();
	std::string APIFUNC_4();

	static API* getSingletonPtr();
private:
	static API* api;
};

#endif