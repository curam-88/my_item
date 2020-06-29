#ifndef FUNCTION_H
#define FUNCTION_H
#include <string>
#include <iostream>
#include <time.h>
#include <sstream>
#include <vector>
#include "DES3.h"
char chr[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',

		'A', 'B', 'C', 'D', 'E', 'F', 'G', 

		'H', 'I', 'J', 'K', 'L', 'M', 'N',

		'O', 'P', 'Q', 'R', 'S', 'T', 

		'U', 'V', 'W', 'X', 'Y', 'Z',

		'a', 'b', 'c', 'd', 'e', 'f', 'g',

		'h', 'i', 'j', 'k', 'l', 'm', 'n',

		'o', 'p', 'q', 'r', 's', 't', 

		'u', 'v', 'w', 'x', 'y', 'z'};
std::vector<std::string> subStrToVec(std::string str,char sep){
	std::vector<std::string> vecArr;
	int flagSub = 0;
	for(int i=0;i<str.size();i++){
		if(str[i] == sep){
			std::string temp = str.substr(flagSub,i-flagSub);
			vecArr.push_back(temp);
			flagSub = i+1;
		}
		if(i == str.size()-1){
			std::string temp = str.substr(flagSub,i-flagSub+1);
			vecArr.push_back(temp);
			flagSub = i+1;
		}
	}

	return vecArr;
}
long int getlongint(std::string str){
	std::stringstream ss;
	ss << str;
	long int times;
	ss >> times;
    return times;
}
std::string getTime()
{
    time_t timep;
    time (&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y%m%d%H%M%S",localtime(&timep) );
    return tmp;
}
bool Check_Time(std::string time, std::string LF){
	long int time1= getlongint(time);
	long int LF1 = getlongint(LF);
	long int now = getlongint(getTime());
	if(now - time1 > LF1)
		return false;
	return true;
}
std::string padding(std::string str){
	int n = str.size();
	if(n < 16){
		for(int i = 0; i < 16-n;i++)
			str += '0';
	}
	return str;
}
std::string randomkey(){
	/*srand(time(NULL));
	std::string str = "";
	for (int i=0; i<16; i++)
	{
		int idx = rand()%62;
		str += chr[idx];
	}*/
	return jiami();
}
#endif