#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include <dirent.h>
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

    if(listen(sock,10) < 0){//监听
        perror("listen");
        exit(4);
    }

    return sock;//返回监听套接字
}
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
int main(int argc,char* argv[])
{
	char path[100]="/home/ly/unixprogram/5/sever";
    //端口号和ip；
    daemon(0,0);
    chdir(path);
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

        printf("get a connect,ip is %s,port is %u\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));

        pid_t id = fork();
        if(id < 0){
            perror("fork");
            close(new_sock);
        }
        else if(id == 0)
        {//子进程
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
             ssize_t s = read(new_sock,buf,sizeof(buf));

             if(s > 0){//读取成功
                int back;
                 buf[s] = '\0';
                 printf("client#:%s\n",buf);
                 back=judge(buf);//判断命令
                 
                 if(back ==  1)//收到hello
                 {
                    memset(buf,0,1024);
                    strcpy(buf,"connected");
                    write(new_sock,buf,strlen(buf));//写到套接字中
                 }
                 else if(back == 2)//收到list
                 {
                    memset(buf,0,1024);
                    //treval(buf);
                    DIR * dir;
                    char a[10]="  \n";
				    struct dirent * ptr;
				    //getcwd(path,100);
				    //int i;
				    dir = opendir(path);
				    while((ptr = readdir(dir)) != NULL)
				    {
				    	write(new_sock,a,strlen(a));//写到套接字中
				    	write(new_sock,ptr->d_name,strlen(ptr->d_name));//写到套接字中
				    	
				        //printf("d_name : %s\n", ptr->d_name);
				    }
				    closedir(dir);
                 }
                 else if(back ==3)//other
                 {
                    memset(buf,0,1024);
                    strcpy(buf,"invalid instruction");
                    write(new_sock,buf,sizeof(buf));//写到套接字中
                 }
                 else if(back == 4)//put
                 {
                         char gfilname[100];
                         memset(gfilname,0,100);
                         strcpy(gfilname,buf);
                         for(int i=0;i<100;i++)
						{
							if(i>3)
							{
								gfilname[i-4]=gfilname[i];
							}
							if(gfilname[i]==0)
								{break;}
						}
                        memset(buf,0,1024);
                        int b=access(gfilname,R_OK);
                        if(b<0)
                        {
                            strcpy(buf,"no file");
                             write(new_sock,buf,sizeof(buf));//写到套接字中
                        }
                        else
                        {
                        	 strcpy(buf,"OK");
                             write(new_sock,buf,sizeof(buf));//写到套接字中
                            int fd=open(gfilname,O_RDONLY);
                            if(fd<0)
                            {
                                strcpy(buf,"open file error");
                                 write(new_sock,buf,sizeof(buf));//写到套接字中
                            }
                            else
                            {
                            	char a[10]="end";
                            	memset(buf,0,sizeof(buf));
                                while((read(fd,buf,1024))>0)
                                {write(new_sock,buf,sizeof(buf));}//写到套接字中
                            	write(new_sock,a,sizeof(a));        
                            }
                    	}
                 }
                 else if(back == 5)//get
                 {
                 		 int fd;
                 	     char gfilname[100];
                         memset(gfilname,0,100);
                         strcpy(gfilname,buf);
                         for(int i=0;i<100;i++)
						{
							if(i>3)
							{
								gfilname[i-4]=gfilname[i];
							}
							if(gfilname[i]==0)
								{break;}
						}
                        memset(buf,0,1024);
                         read(new_sock,buf,sizeof(buf));
                   if(buf[0] =='O')
                   {
	                   fd = open(gfilname,O_CREAT|O_RDWR,0755);
	                   if(fd>0)
	                   {
	                    	while(1)
	                    	{
	                        read(new_sock,buf,sizeof(buf));
	                        if(strcmp(buf,"end")==0)
	                            break;
	                        write(fd,buf,strlen(buf));
	                        memset(buf,0,sizeof(buf));
	                   	    }
                  	    }

                	}
            	}

                // write(new_sock,buf,strlen(buf));//写到套接字中
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
        {//父进程
         close(new_sock);
         waitpid(id,NULL,0);
        }
    }
    return 0;
}

