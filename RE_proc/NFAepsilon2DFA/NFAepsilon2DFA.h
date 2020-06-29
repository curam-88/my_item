#ifndef NFAEPSILON2DFA_H
#define NFAEPSILON2DFA_H

#include <iostream>
#include <fstream>
#include <set>
#include <stack>
#include <map>
#include <bits/stdc++.h>

#define MAXSIZES 1500
struct state_table//存放映射表
{
    size_t new_state;
    std::set<size_t> old_state; 
};
struct new_detal//存放新的状态转移函数
{
    size_t start;
    std::set<char> change;
    size_t end;
};
class NFAepsilon2DFA{
	private:
        //NFAepsilon2DFA的结构
		std::set<size_t> Q;
    	std::set<char> Sigma;
    	std::multimap<std::pair<size_t, std::set<char>>, size_t> Delta;
    	size_t Q0;
    	size_t F;

        state_table *Q_to = new state_table[MAXSIZES];//映射表
        
        //NFA的结构
        std::set<size_t> Qt;
        std::set<char> Sigmat;
        std::set<size_t> Q0t;
        std::set<size_t> Ft;
        new_detal *detal = new new_detal[MAXSIZES];//状态转移函数
    public:
    	NFAepsilon2DFA();
    	~NFAepsilon2DFA();
    	void ReadNFAepsilon(std::string file_name);//读取文件
    	void SetNFA();//设置NFA
        void Eclouse(size_t c,std::set<size_t> &res);//跳空单个状态
        std::set<size_t> DeleteAepsilon(std::set<size_t> Q_all);//跳空状态集合
        std::string PrintSet(std::set<char> charset);//set转为string
        std::string FilePrintSet(std::set<char> charset);
        std::set<size_t> InWord(std::set<size_t>,char word);//输入字符返回状态集合
        bool EqualSet(std::set<size_t> Q1,std::set<size_t> Q2);//比较set
        void OutPut(int state_size,int detal_size);//确定Q0t，Ft
        void FILEOUT(int state_size,int detal_size);//输出到文件中
};
#endif