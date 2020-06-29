#include "../SOCKET/server.h"
#include "SERVER_V.h"
int main()
{
	Server sock;
	char ip[20] = "127.0.0.1";
	sock.buildserver(ip, 3333);
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
			SERVER_v server_v(sock.new_sock, "qpo1230000000000", "3333");
			server_v.Connect_c();
			exit(0);
		}
		else//父进程
		{
			close(sock.new_sock);
			//waitpid(pid, NULL, 0);
		}
	}
	return 0;
}