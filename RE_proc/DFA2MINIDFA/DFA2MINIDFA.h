#ifndef DFA2MINIDFA_H_
#define DFA2MINIDFA_H_

#include <iostream>
#include <map>
#include <fstream>
#include <set>

class DFA2MINIDFA
{
private:
    std::set<size_t> Q;             //状态集
    std::set<char> Sigma;           //字母表
    std::size_t Q0;                    //初始状态
    std::set<size_t> F;                //终止状态集
    std::map<std::pair<size_t, char>, size_t> Delta;  //状态转移函数

    std::set<size_t> minQ;             //状态集
    std::set<char> minSigma;           //字母表
    std::size_t minQ0;                    //初始状态
    std::set<size_t> minF;                //终止状态集
    std::multimap<std::pair<size_t, char>, size_t> minDelta;  //状态转移函数
public:
    DFA2MINIDFA();
    ~DFA2MINIDFA();
    void ReadDFA(std::string filename);
    size_t Move(size_t qBegin, char transchar);
    std::set<size_t> SetCount(std::map<size_t, size_t> state2set);
    std::set<std::set<size_t>> Split();
    void GetMiniDFA(std::set<std::set<size_t>> Pai_initial);
    void Show(std::set<std::set<size_t>> Pai_initial);
    void OutputMiniDFA2File();
};

#endif