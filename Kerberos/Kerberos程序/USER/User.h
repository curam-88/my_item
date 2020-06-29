#ifndef USER_H
#define USER_H
#include <iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sstream>
#include <vector>
struct TS
{
	std::string Ts1;
	std::string Ts2;
	std::string Ts3;
	std::string Ts4;
	std::string Ts5;
	std::string LF2;
	std::string LF4;
};
class User{
public: 
	int sock;
	int sock_Tsg;
	int sock_Ser;

	std::string password;
	std::string IDc;
	std::string IDtgs;
	std::string IDv;
	
	TS ts;

	std::string Ticket;
	std::string Kc_v;
	std::string Kc_tgs;
	std::string TGT;
	std::string Kc;
	User(int s);
	void Login_As(std::string IDtgs, std::string IDc, std::string pass);
	void Connect_Tgs(std::string IDv);
	void Connect_V();
	void Write_Message(std::string str);
	std::string Read_Message();
};
#endif