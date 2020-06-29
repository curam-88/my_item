#include "../SOCKET/socket.h"
#include "User.h"
int main(){
	//Server a;
	Sock_Client client;
	char ip[50] = "127.0.0.1";
	client.bulidsocket(ip, 1234);
	User user(client.sock);
	std::cout << "请输入用户的ID:";
	std::string ID;
	std::string password;
	std::cin >> ID;
	std::cout << "请输入用户的密码:";
	std::cin >> password;
	user.Login_As(ID, "peng" , password);
	close(client.sock);
	client.bulidsocket(ip, 2222);
	user.sock = client.sock;
	std::cout << "请选择服务:\n" << "1.查看\n" << "2.退出\n" << std::endl;
	std::string IDv;
	int n;
	std::cin >> n;
	if(n == 1){
		IDv = "3333";
	} 
	else if(n == 2){
		exit(0);
	}
	else{
		std::cout << "输入异常退出!\n";
	}
	user.Connect_Tgs(IDv);
	close(client.sock);
	client.bulidsocket(ip, 3333);
	user.sock = client.sock;
	user.Connect_V();
	
//下面三基本通信方式
/*while(1)
	{
		char buf[1024];
		std::cout << "please enter:";
		fflush(stdout);
		ssize_t s = read(0,buf,sizeof(buf)-1);
		if(s>0)
		{
			buf[s-1] = 0;
			write(client.sock,buf,strlen(buf));
			s = read(client.sock,buf,sizeof(buf)-1);
			
			if(s > 0)
			{
				buf[s] = 0;
				printf("server says:%s\n",buf);
			}
		}
	}*/
//用户具体代码写在下方,socket接口是client.sock
	
	return 0;
}