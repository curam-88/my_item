#ifndef KDC_H_
#define KDC_H_

/***********************************************
	需求：
	2 登记并管理下属服务器密钥
	4 签发访问凭证 - done
	5 验证用户身份 - done
	6 可被客户端检测
	
************************************************/
#include<string>
#include<cstring>
#include<errno.h>
#include<ctime>
#include<iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <fcntl.h>
#include <signal.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <vector>
#include "../AES/aes.h"
#include "../function.h"
#include "../server.h"
#include "../function.h"
#define Max 256

class User {
	string ID;
	string K;
};
/*
这个函数用于判断用户是否存在；
*/
bool isExist(string str) {
	char buffer[Max];
	ifstream in("message.txt");
	if (!in.is_open()) {
		cout << "文件打开错误！";
		exit(1);
	}
	while (!in.eof()) {
		in.getline(buffer, 200);
		char* p = strtok(buffer, ",");
		while (p) {
			//把文件中的id解密后与输入进行比较
			//string s = DecryptionAES(p);
			//cout << s << endl;
			if (p == str) {
				return true;
			}
			else {
				break;
			}
		}
	}
	cout << "用户 " <<str<<" 不存在 " << endl;
	return false;
}
//实现对用户的注册，需要用户的id和pwd
bool regist(string id, string pwd)
{
	//对用户id进行加密
	//string miwen_id = jiami(id);
	//string miwen_pwd = jiami(pwd);
	//string miwen_id = EncryptionAES(id);
	//string miwen_pwd = EncryptionAES(pwd);
	if (isExist(id)) {
		cout << "用户id 已存在" << endl;
		return false;
	}
	ofstream out("message.txt",ios::app);
	string str = id + "," + pwd;
	out << str << endl;
	out.close();
	return true;
}
//加密，按用户提供的密钥加密
//KDC向用户发送的Ticket（tgs）中使用tgs的密钥进行加密
//tgs 是TGS的id，str是用户想要加密的内容
//str 包括 Kc,tgs + IDc + ADc + IDtgs + TS2 + LF2
string GetTickt(string session_key,string idc,string ADc,string tgs,string ts,string lf)
{
	string str = session_key + "|" + idc + "|" + ADc + "|" + tgs + "|" + ts + "|" + lf;
	char buffer[Max];
	ifstream in("message.txt");
	if (!in.is_open()) {
		cout << "文件打开错误！";
		exit(1);
	}
	while (!in.eof()) {
		in.getline(buffer, 200);
		char* p = strtok(buffer, ",");
		while (p) {
			//把文件中的id解密后与输入进行比较
			//cout << s << endl;
			//string s1 = DecryptionAES(p);
			if (p == tgs) {
				p = strtok(NULL, ",");
				//string s = DecryptionAES(p); //取得tgs的密钥
				int len = strlen(p);
				char t[Max];
				for (int i = 0; i < strlen(p)-1; i++) {
					t[i] = p[i];
				}
				string k = t;
				//char* k = t;
				k = padding(k);
				string ticket = EncryptionAES(str, (char*)k.data());//用tgs的密码充当密钥加密
				//cout << str << endl;
				//cout << ticket << endl;
				//string rs = DecryptionAES(ticket, k);
				//cout << rs << endl;
				return ticket;
			}
			else {
				break;
			}
		}
	}
	cout << "该TGS不存在" << endl;
}
/*
//获得当前时间
//long int getTime() 
{
	time_t t;
	time(&t);
	return (long int)t;
}//
*/
//获得 用户C 和 认证服务器KDC的会话密钥
string getSessionKey()
{
	string session_key = randomkey();
	return session_key;
}
//获得最后要发送给用户 C 的密文，用户C 的密码充当密钥
string getContent(string session_key,string idc,string ts,string lf,string ticket)
{
	string content = session_key + "|" + idc + "|" + ts + "|" + lf + "|" + ticket;
	char buffer[Max];
	ifstream in("message.txt");
	if (!in.is_open()) {
		cout << "文件打开错误！";
		exit(1);
	}
	while (!in.eof()) {
		in.getline(buffer, 200);
		char* p = strtok(buffer, ",");
		while (p) {
			//把文件中的id解密后与输入进行比较
			//string s1 = DecryptionAES(p);
			if (p == idc) {
				p = strtok(NULL, ",");
				//string s = DecryptionAES(p); //取得用户c 的密钥
				int len = strlen(p);
				char t[Max];
				for (int i = 0; i < strlen(p)-1; i++) {
					t[i] = p[i];
				}
				string k = t;
				k = padding(k);
				cout << k.size() << endl;
				//char* k = t;
				cout << "加密密钥:" << k << endl;
				string miwen_content = EncryptionAES(content, (char*)k.data());
				string rs = DecryptionAES(miwen_content,(char*)k.data());
				cout << "解密:" << rs << endl;
				return miwen_content;
			}
			else {
				break;
			}
		}
	}
	cout << "该用户不存在" << endl;
}
#endif // !__KDCSERVER_H_
