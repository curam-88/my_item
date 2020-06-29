#include "KDC.h"
#define N 1000
int main()
{
	string id;
	string pwd ;
	//string miwen_id = jiami(id);
	//jiemi(miwen_id);
	//string miwen_pwd = jiami(pwd);
	//jiemi(miwen_pwd);
	/*
	
	cout << "请输入用户名： " << endl;
	cin >> id ;
	cout << "请输入密码：" << endl;
	cin >> pwd;
	if (regist(id, pwd)) {
		cout << "注册成功" << endl;
	}
	
	string idc = "lanjinglin";
	string adc = "127.0.0.1";
	string session_key = getSessionKey();
	string tgs = "peng";
	string TS = to_string(getTime());
	string LF = "1500";
	string ticket = GetTickt(session_key, idc, adc, tgs, TS, LF);
	string content = getContent(session_key, idc, TS, LF, ticket);
	//cout << content << endl;
	*/
	Server sock;
	char ip[20] = "127.0.0.1";
	sock.buildserver(ip, 1234);
	string LF = "1500";
	string ADc = "127.0.0.1";
	while (sock.serveraccept() > 0) {
		pid_t pid = fork();
		if (pid < 0) {
			perror("fork");
			close(sock.new_sock);
		}
		else if (pid == 0) {
			close(sock.sock);
			if (fork() > 0)
				exit(0);
			while (1) {
				char buf[1024];
				memset(buf, 0, sizeof(buf));
				ssize_t s = read(sock.new_sock, buf, sizeof(buf));
				if (s > 0) {
					cout <<"接收到Client 的发来的消息"<<endl;
					cout << buf << endl;
					vector<string> v = subStrToVec(buf, '|');
					string IDc = v[0];
					string IDtgs = v[1];
					string TS1 = v[2];
					//检查用户 c 和 TGS是否存在
					if(!isExist(IDc) || !isExist(IDtgs))
						exit(0);
					//检查是否过期
					if(! Check_Time(TS1, LF)){
						cout <<"已过期！"<<endl;
						exit(0);
					}
					string TS2 = getTime();
					//获得会话密钥
					string session_key = getSessionKey();
					//得到加密后的ticket
					string ticket = GetTickt(session_key, IDc, ADc, IDtgs, TS2, LF);
					//得到最后要发送给Client 的内容
					string content = getContent(session_key, IDc, TS2, LF, ticket);
					cout << "向Cilent发送加密消息" << endl<<content<<endl;
					char b[1024];
					strcpy(b, content.c_str());
					//cout << endl << b << endl;
					write(sock.new_sock, b, sizeof(b) + 1);
				}
				else if (s == 0) {
					cout << "client exit!" << endl;
					break;
				}
			}
		}
		else {
			close(sock.new_sock);
		}
	}
	return 0;
}
