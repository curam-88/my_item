#ifndef SOKET_H
#define SOKET_H
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
class Sock_Client{
public:
int sock;
Sock_Client()
{

}
int bulidsocket(const char *host, int clientprot)
{
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock<0){
		perror("socket");
	return -1;
	}
struct  sockaddr_in peer;
peer.sin_family = AF_INET;
peer.sin_port = htons(clientprot);
peer.sin_addr.s_addr = inet_addr(host);
if(connect(sock,(struct sockaddr*)&peer,sizeof(peer))<0)
	{
	perror("connect");
	return -1;
	}
	this->sock = sock;
	return 0;
}
};
#endif