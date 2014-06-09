#ifndef SCRIPTLAYER_H
#define SCRIPTLAYER_H

#include "stdafx.h"

class ScriptLayer{
public:
	ScriptLayer();
	~ScriptLayer();

	void startTests();

	void parseLine(std::string line);

};

#endif