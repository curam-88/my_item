#ifndef SERVER_H
#define SERVER_H
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
class Server {
public:
	int sock;
	int new_sock;
Server()
{

}
public: int buildserver(char *ip, int port) {
	 //创建套接字
    sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock < 0){
        perror("socket");
        exit(2);
    }   
    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port = htons(port);
    local.sin_addr.s_addr = inet_addr(ip);
    if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0){//bind
        perror("bind");
        exit(3);
    }
    if(listen(sock,10) < 0){//监听
        perror("listen");
        exit(4);
    }
    this->sock = sock;
    return sock;//返回监听套接字
}
public: int serveraccept()
{
	struct sockaddr_in client;
	socklen_t len = sizeof(client);
	this->new_sock = accept(this->sock, (struct sockaddr*)&client, &len);
	if(new_sock < 0)
		return -1;
	//std::cout << "get a connect,ip is "<< inet_ntoa(client.sin_addr) <<",port is "<<ntohs(client.sin_port); 
	return new_sock;
}
};
#endif