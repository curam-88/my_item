#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<fcntl.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int getname(char *name)
{
  for(int i=0;i<100;i++)
{
    if(i>3)
    {
        name[i-4]=name[i];
    }
    if(name[i]==0)
        {break;}
}
}
int judge(char *buf)
{
    if(strcmp(buf,"hello")==0)
    {
        return 1;
    }
    else if(strcmp(buf,"list")==0)
    {
       return 2;
    }
    else if(buf[0]=='g'&&buf[1]=='e'&&buf[2]=='t')
    {
        return 4;
    }
    else if(buf[0]=='p'&&buf[1]=='u'&&buf[2]=='t')
    {
        return 5;       
    }
    else
    {
        return 3;
    }
}
static void Usage(const char* proc)
{
    printf("Usage:%s[server_ip][server_prot]\n",proc);
}

int main(int argc,char* argv[])
{
    char *addr="127.0.0.1";
    char *port="1234";

    int sock = socket(AF_INET,SOCK_STREAM,0);//创建socket
    if(sock < 0){
        perror("socket");
        return 2;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(port));
    server.sin_addr.s_addr = inet_addr(addr);

    if(connect(sock,(struct sockaddr*)&server,sizeof(server)) < 0){//连接
           perror("connect");
        return 3;
    }

    while(1)
    {//先写后读
         printf("client#");
         fflush(stdout);
         char buf[1024];
         memset(buf,0,1024);
         ssize_t s = read(0,buf,sizeof(buf));//读取数据
         //judge(buf)；
         if(s > 0)
         {
             buf[s-1] = '\0';
             int back = judge(buf);
             write(sock,buf,sizeof(buf));
             if(back == 1)
             {
             ssize_t _s = read(sock,buf,sizeof(buf));//读sock
               printf("server echo#%s\n",buf);
             }
             else if(back == 2)
             {
                ssize_t _s = read(sock,buf,sizeof(buf));
                printf("server echo#%s\n",buf);

             }
             else if(back == 3)
             {
                ssize_t _s = read(sock,buf,sizeof(buf));//shi xian server hui xian
                printf("server echo#%s\n",buf);
             }
             else if(back == 4)
             {
                printf("1\n");
                int fd;
                char name[100];
                strcpy(name,buf);
                 for(int i=0;i<100;i++)
                    {
                        if(i>3)
                        {
                            name[i-4]=name[i];
                        }
                        if(name[i]==0)
                            {break;}
                    }
                memset(buf,0,1024);
                read(sock,buf,sizeof(buf));
                 printf("2\n");
                if(buf[0] =='O')
                {
                    fd = open(name,O_CREAT|O_RDWR,0755);
                   if(fd>0)
                   {
                    while(1)
                    {
                        read(sock,buf,sizeof(buf));
                        if(strcmp(buf,"end")==0)
                            break;
                        write(fd,buf,strlen(buf));
                        memset(buf,0,sizeof(buf));
                    }
                   }
                  else if(buf[0]=='n')
                  {
                    printf("no file");
                  }
                }
             }
             else if(back == 5)
             {
                int fd;
                char name[100];
                strcpy(name,buf);
                 for(int i=0;i<100;i++)
                    {
                        if(i>3)
                        {
                            name[i-4]=name[i];
                        }
                        if(name[i]==0)
                            {break;}
                    }
                memset(buf,0,1024);
                char a[10]="end";
                memset(buf,0,sizeof(buf));
                fd = open(name,O_CREAT|O_RDWR,0755);
                strcpy(buf,"OK");
                write(sock,buf,sizeof(buf));//写到套接字中
                while((read(fd,buf,1024))>0)
                {write(sock,buf,sizeof(buf));}//写到套接字中
                write(sock,a,sizeof(a));
            }
           }
         else if(s == 0){
             printf("read ending\n");
             exit(4);
         }
         else{
             perror("read");
             break;
         }
    }
    close(sock);
    return 0;
}
