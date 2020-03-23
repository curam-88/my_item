#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
static void usage(const char *proc)
{printf("Usage:%s[ local_ip] [local_port]\n",proc);}
int main(int argc,char *argv[])
{
	if(argc!=3)
	{
		usage(argv[0]);
		return 1;}	
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock<0){
		perror("socket");
	return 2;}
struct  sockaddr_in peer;
peer.sin_family = AF_INET;
peer.sin_port = htons(atoi(argv[2]));
peer.sin_addr.s_addr = inet_addr(argv[1]);
if(connect(sock,(struct sockaddr*)&peer,sizeof(peer))<0)
	{
	perror("connect");
	return 3;}
	char buf[1024];
	while(1)
	{
		printf("please enter:");
		fflush(stdout);
		ssize_t s = read(0,buf,sizeof(buf)-1);
		if(s>0)
		{
			buf[s-1] = 0;
			write(sock,buf,strlen(buf));
			s = read(sock,buf,sizeof(buf)-1);
			
			if(s > 0)
			{
				buf[s] = 0;
				printf("server says:%s\n",buf);
			}
		}
	}
	return 0;
}