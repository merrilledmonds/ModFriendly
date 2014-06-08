#include "stdafx.h"
#include "API.h"

API* API::api;

API::API(){

};
API::~API(){};


void API::APIFUNC_1(){
	std::cout<<"API Function 1\n";
};
int API::APIFUNC_2(){
	std::cout<<"API Function 2\n";
	return 10;
};
bool API::APIFUNC_3(){
	std::cout<<"API Function 3\n";	
	return true;
};
std::string API::APIFUNC_4(){
	std::cout<<"API Function 4\n";
	return "API String";
};


API* API::getSingletonPtr(){
	if(!api){
		api = new API();
	}
	return api;
};