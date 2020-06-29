#include "User.h"
//#include "./TimeWay/TimeWay.h"
#include "../function.h"
#include "../AES/aes.h"
User::User(int s){
	this->sock = s;
}
void User::Login_As(std::string IDc, std::string IDtgs, std::string pass){
	std::cout << "-------------------Client与AS建立连接--------------------\n\n" << std::endl; 
	this->IDtgs = IDtgs;
	this->IDc = IDc;
	this->password  = pass;
	this->Kc = padding(pass);
	cout << this->Kc << "\n";
	std::string Message = this->IDc + '|' + this->IDtgs + '|' + getTime();
	this->Write_Message(Message);
	std::cout << "Client向AS发送消息: " << Message << std::endl; 

	std::string read_as = Read_Message();
	//解密
	std::string as = DecryptionAES(read_as, (char*)Kc.c_str());
	std::vector<std::string>As = subStrToVec(as, '|');
	std::string tt = As[0];
	std::cout << "\nClinet解密得到的消息:" << as << std::endl;
	this->ts.Ts2 = As[2];
	this->ts.LF2 = As[3];
	if(!Check_Time(this->ts.Ts2, this->ts.LF2)){
		std::cout << "Error~";
  	}
	this->Kc_tgs = As[0];
	this->TGT = As[4];
	this->IDtgs = As[1];
	std::cout << "LF2: " << this->ts.LF2 << std::endl;
	std::cout << "TS2: " << this->ts.Ts2 << std::endl;
	std::cout << "Kc_tgs: " << this->Kc_tgs << std::endl;
	std::cout << "TGT: " << this->TGT << std::endl;
	std::cout << "IDtgs: " << this->IDtgs << std::endl;
	std::cout << "-------------------Client与AS断开连接连接--------------------\n\n" << std::endl; 
}
void User::Connect_Tgs(std::string IDv){
	std::cout << "-------------------Client与TGS建立连接--------------------\n" << std::endl; 
	this->IDv = IDv;
	std::string auth = this->IDc + '|' + "127.0.0.1" + '|' + getTime();  
	//加密
	auth = EncryptionAES(auth,(char*)Kc_tgs.c_str());
	std::string str = IDv + '|' + this->TGT + '|' + auth;
	std::cout<< "client向TGS发送消息:" << str << "\n";
	Write_Message(str);

	std::string tgsMessage = DecryptionAES(Read_Message(), (char*)Kc_tgs.c_str());
	std::cout<< "client接收TGS消息并解密: " << tgsMessage << "\n";
	std::vector<std::string> Tgs = subStrToVec(tgsMessage, '|');
	this->Kc_v = Tgs[0];
	if(this->IDv != Tgs[1])
		std::cout << "error!" << std::endl;
	this->ts.Ts4 = Tgs[2];
	this->Ticket = Tgs[3];
	std::cout << "Kc_v: " << Tgs[0] << std::endl;
	std::cout << "Ts4: " << Tgs[2] << std::endl;
	std::cout << "Ticket: " << Tgs[3] << std::endl;
	std::cout << "-------------------Client与TGS断开连接--------------------\n" << std::endl; 
}
void User::Connect_V(){
	std::cout << "-------------------Client与服务器建立连接--------------------\n" << std::endl; 
	this->ts.Ts5 = getTime();
	std::string auth = this->IDc + '|' + "127.0.0.1" + '|' + this->ts.Ts5;
	auth = EncryptionAES(auth,(char*)Kc_v.c_str());
	//用this->Kc_v加密auth
	std::string str = this->Ticket + '|' +  auth;
	Write_Message(str);
	std::string serverMessage = DecryptionAES(Read_Message(), (char*)Kc_v.c_str());
	std::cout << "server: " << serverMessage << std::endl;
	std::cout << "-------------------Client完成验证～--------------------\n" << std::endl; 
}
void User::Write_Message(std::string str){
	char b[1024];
 	strcpy(b,str.c_str());
 	write(sock,b,sizeof(b));//写到套接字中
}
std::string User::Read_Message(){
	//std::string str;
	char buf[1024];
	memset(buf, 0, sizeof(buf));
	int s = read(this->sock, buf, sizeof(buf)+1);
	//std::cout << buf; 
	std::string str = buf;
		//str += buf;
	std::cout << "Client接收到的消息：" << str << "\n";
	return str;
}