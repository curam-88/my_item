#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include <sys/wait.h>
int StartUp(const char* ip,int port)
{
    //创建套接字
    int sock = socket(AF_INET,SOCK_STREAM,0);

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

    if(listen(sock,10) < 0){//listen
        perror("listen");
        exit(4);
    }

    return sock;//return an "listen_sock";
}

int main(int argc,char* argv[])
{
    //端口号和ip；
    char *addr="127.0.0.1";
    char *port="1234";
    //监听套接字；
    int listen_sock = StartUp(addr,atoi(port));
    while(1)
    {

        struct sockaddr_in client;
        socklen_t len = sizeof(client);

        //创建连接套接字
        int new_sock = accept(listen_sock,(struct sockaddr*)&client,&len);
        if(new_sock < 0){
            //失败重传
            perror("accept");
            continue;
        }

        printf("get a connect,ip is %s,port is %u\n",\
              inet_ntoa(client.sin_addr),ntohs(client.sin_port));

        pid_t id = fork();
        if(id < 0){
            perror("fork");
            close(new_sock);
        }
        else if(id == 0)
        {//child
            close(listen_sock);
            //再一次创建子进程再退出形成孤儿进程
            if(fork() > 0)
            {
             exit(0);
             }   
            //孤儿进程
            while(1)
            {//先读后写
             char buf[1024];
             ssize_t s = read(new_sock,buf,sizeof(buf)-1);

             if(s > 0){//读取成功
                int back;
                 buf[s] = '\0';
                 printf("client#:%s\n",buf);
                 back=judge(buf);
                 if(back ==  1)//收到hello
                 {

                 }
                 else if(back == 2)//收到list
                 {

                 }
                 else if(back == 3)//其他
                 {
                  
                 }
                 else//收发文件
                 {
                   
                 }
                 write(new_sock,buf,strlen(buf));//写到套接字中
             }
             else if(s == 0 ){//读到文件末端
                 printf("client exit\n");
                 break;
             }
             else{//读取错误
                 perror("read");
                 break;
             }
            }
             close(new_sock);
             exit(0);
        }
        else
        {//father
         close(new_sock);
         waitpid(id,NULL,0);
        }
    }
    return 0;
}

