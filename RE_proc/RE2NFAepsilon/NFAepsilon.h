#ifndef NFAEPSILON_H
#define NFAEPSILON_H

#include <iostream>
#include <fstream>
#include <set>
#include <stack>
#include <map>

#define MAXSIZE 256

struct Edge
{
	size_t StartState;
	size_t EndState;
	std::set<char> CharSet;
};

struct Unit
{
	Edge EdgeSet[MAXSIZE];
	size_t EdgeCount;
	size_t StartState;
	size_t EndState;
};

class NFAepsilon
{
private:
    size_t StateCount;
    std::set<size_t> Q;
    std::set<char> Sigma{'#'};
    std::multimap<std::pair<size_t, std::set<char>>, size_t> Delta;
    size_t Q0;
    size_t F;
    std::set<char> epsilon{'#'};
    std::map<char, std::set<char>> contrast;
public:
    NFAepsilon(std::map<char, std::set<char>> contrast);
    ~NFAepsilon();
    Unit Reg2NFAepsilon(std::string rpn);
    Unit Union(Unit left, Unit right);
    Unit Kleene1(Unit left);
    Unit Kleene2(Unit left);
    Unit Join(Unit left, Unit right);
    Unit MakeUnit(char ch);
    size_t NewState();
    void AddEdge2UnitFromUnit(Unit& dest, Unit src);
    std::set<char> GetSetFromMap(char ch);
    std::string PrintSet(std::set<char> charset);
    void Display(Unit unit);
    void NFAepsilonSummary(Unit unit);
    void Output(std::string filename);
};

#endif