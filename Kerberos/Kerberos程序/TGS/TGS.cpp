#include "../SOCKET/server.h"
#include "TGS.h"
int main()
{
	Server sock;
	char ip[20] = "127.0.0.1";
	sock.buildserver(ip, 2222);
	while(sock.serveraccept() > 0)
	{
		pid_t pid = fork();
		if(pid < 0)
		{
			perror("fork");
			close(sock.new_sock);
		}
		else if(pid == 0)
		{
			close(sock.sock);
			if(fork() > 0)
				exit(0);
			std::string t = "qpo1230000000000";
			TGS tgs(sock.new_sock, t, "peng");
			tgs.Connect_c();
			close(sock.new_sock);
			return 0; 
			/*while(1)
			{
			 char buf[1024];
             ssize_t s = read(sock.new_sock,buf,sizeof(buf));
             if(s > 0){//读取文件
             	std::cout << buf <<std::endl;
             	write(sock.new_sock,buf,strlen(buf));//写到套接字中
             }
             else if(s == 0){//读到文件末端
             	std::cout << "client exit!" <<std::endl;
             	break;
             }
             else{
             	perror("error in readclient");
             	break;
             }
			}*/
			//TGS操作写在下方

		}
		else//父进程
		{
			close(sock.new_sock);
			waitpid(pid, NULL, 0);
		}
	}
	return 0;
}