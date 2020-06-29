#ifndef RE_H
#define RE_H
#include <iostream>
#include <iostream>
#include <cstdlib>
#include <stack>
#include <map>
#include <set>
class RE
{
private:
    std::string regexp;
public:
    RE(std::string str_in);
    ~RE();
    std::map<char, std::set<char>> Parse(std::string regexp, std::string &reg_parsed);
    std::string convert(std::string regexp, std::map<char, std::set<char>> &contrast);
    std::set<char> GetSet(std::string tempstr);
    size_t GetPriority(char ch);
};

#endif
