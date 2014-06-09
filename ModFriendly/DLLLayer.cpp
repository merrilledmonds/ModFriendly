#include "stdafx.h"
#include "DLLLayer.h"

DLLLayer::DLLLayer(){

};
DLLLayer::~DLLLayer(){

};

void DLLLayer::startTests(){
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
};