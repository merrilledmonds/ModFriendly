// ModFriendlyDll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ModFriendlyDll.h"

namespace ModFriendlyLib{

	bool simpleCheck(){
		return true;
	};

	int simpleCalculation(int a, int b){
		return a+b;
	};

	void simpleOutput(){
		std::cout << "        Simple Function Check Using Library: Successful. (OK)" << std::endl;
	};

	SimpleClass::SimpleClass(){
		intMember = 1;
	};

	SimpleClass::~SimpleClass(){
	};

	int SimpleClass::functionMember(int a){
		return a + intMember;
	};

}