#include "SERVER_V.h"
#include "../AES/aes.h"
#include "../function.h"
SERVER_v::SERVER_v(int sock, std::string Kv, std::string IDv){
	this->sock = sock;
	this->Kv = Kv;
	this->IDv = IDv;
}
SERVER_v::~SERVER_v(){}
void SERVER_v::Connect_c(){
	std::string str = Read_Message();
	std::cout << "服务器接受Cient的消息:" << str <<std::endl;
	//拆分接受到的消息
	std::vector<std::string>K_c = subStrToVec(str, '|');
	std::string Ticket_v = K_c[0];
	std::string auth = K_c[1];
	std::cout << "Ticket_v解密前:" << K_c[0] <<std::endl;
	Ticket_v = DecryptionAES(K_c[0], (char*)Kv.c_str());/* 解密Ticket */
	std::cout << "Ticket_v解密后:" << Ticket_v <<std::endl;
	std::vector<std::string>Ticket = subStrToVec(Ticket_v, '|');/* 拆分Ticket */
	this->Kc_v = Ticket[0];
	this->IDc = Ticket[1];
	this->ADc = Ticket[2];
	if(this->IDv != Ticket[3])
		std::cout << "ERROR in IDv\n"; 
	this->TS4 = Ticket[4];
	this->LF4 = Ticket[5];
	std::cout << Ticket[3] << std::endl;
	if(!Check_Time(Ticket[4], Ticket[5]))/* 判断时间合理性 */
	{
		std::cout << "超时!" << std::endl;
	}
	auth = DecryptionAES(K_c[1], (char*)this->Kc_v.c_str());/* 解密auth */
	std::cout << "auth解密后:" << auth << std::endl;
	std::vector<std::string>Authent = subStrToVec(auth, '|');/* 拆分Authent */
	if(this->IDc != Authent[0])
		std::cout << "IDc error\n";
	if(this->ADc != Authent[1])
		std::cout << "ADc error\n";
	this->TS5 = Authent[2];
	std::string Message = "pass validation!";
	Message = EncryptionAES(Message, (char*)Kc_v.c_str());
	Write_Message(Message);
	std::cout << "发送给Client的消息:" << Message <<std::endl;
	close(this->sock);
}
std::string SERVER_v::Read_Message(){
	char buf[1024];
	memset(buf, 0, sizeof(buf));
	int s = read(this->sock, buf, sizeof(buf));
	//std::cout << buf; 
	std::string str = buf;
		//str += buf;
	std::cout << str << "\n";
	return str;
}
void SERVER_v::Write_Message(std::string str){
	char b[1024];
 	strcpy(b,str.c_str());
 	write(sock,b,sizeof(b));//写到套接字中
}