#ifndef TGS_H
#define TGS_H
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

class TGS{
public:
	int sock;

	std::string Ktgs;
	std::string Kc_tgs;
	std::string IDtgs;
	std::string IDv;
	std::string ADc;
	std::string IDc;
	std::string	TS2;
	std::string LF2;
	std::string	TS3;
	std::string	TS4;
	TGS(int sock, std::string Ktgs, std::string IDtgs);
	~TGS();
	void Connect_c();
	std::string Read_Message();
	void Write_Message(std::string str);
};
#endif