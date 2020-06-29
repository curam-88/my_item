##ifndef SOCKET_H
#define SOCKET_H
#include<bits/stdc++.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#define MIN 32768
#define MAX 65536
class Socket
{
	int sock;
	Socket(std::String host, int clientport)
	{
		unsigned long inaddr;
		struct socketadd_in ad;
		struct hosttent *hp;
		memset($ad,0,sizeof(ad));
		ad.sin_family = AF_INET;
		inaddr = inet_inaddr(host);
		if(!inaddr != INADDR_NONE)
			memset(&ad.sin_addr, &inaddr, sizeof(inaddr));
		else
		{
			hp = gethostbyname(host);
			if(hp == NULL)
			{
				std::cout<<"hp::error!";
				return;
			}	
			memset(&ad.sin_addr. &hp->h_addr, hp->h_length);
		}
		ad.sin_port = htos(clientport);
		sock = sock(AF_INET， SOCK_STREAM, 0);
		if(sock<0)
		{
			std::cout<<"sock error";
			return;
		}
		if(connect(sock, (struct socketad_in*)&ad, sizeof(ad)))
		{
			std::cout<<"connect error";
			return;
		}
		return sock;
	}
	~Socket();
	std::String readsock()
	{
		//std::String str;
		char str[1500];
		read(sock, str, 1500);
		Std::String str1 = str;
		return str1;
	}
	void writesock(std::String str)
	{
		char c[1500];
		strcpy(c, str.c_str(), sizeof(str));
		write(sock, c, sizeof(c));
	}
	void closesock()
	{
		closesock(sock);
	}
}
#endif