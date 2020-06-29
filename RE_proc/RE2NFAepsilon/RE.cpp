#include "RE.h"

RE::RE(std::string str_in)
{
    this->regexp = str_in;
}

RE::~RE(){}

size_t RE::GetPriority(char ch)
{
    size_t priority(0);
    switch (ch)
    {
    case '|': priority = 1; break;
	case '.': priority = 2; break;
	case '*': priority = 3; break;
    case '+': priority = 3; break;
	default:  priority = 0; break;
    }
    return priority;
}

std::set<char> RE::GetSet(std::string tempstr)
{
    std::set<char> res;
    if(tempstr.size() == 1) {
        res.insert(tempstr[0]);
    }
    else {
        char lowerBound = tempstr[0];
        char upperBound = tempstr[2];
        for(size_t i = lowerBound; i <= upperBound; i++) {
            res.insert((char)i);
        }
    }
    return res;
}

std::string RE::convert(std::string regexp, std::map<char, std::set<char>> &contrast)
{
    char begin('A');
    std::string result("\0");
    size_t i(0);
    while(i < regexp.size()) {
        if(regexp[i] != '[') {
            result += regexp[i];
            ++i;
        }
        else {
            ++i;  //跳过[
            std::string tempstr("\0");
            while(regexp[i] != ']') {
                tempstr += regexp[i];
                ++i;
            }
            //...
            result += begin;
            contrast.insert(std::pair<char, std::set<char>>(begin, GetSet(tempstr)));
            ++begin;
            ++i;  //]
        }
    }
    return result;
}

std::map<char, std::set<char>> RE::Parse(std::string regexp, std::string &reg_parsed)
{
    std::stack<char> s1, s2;
    std::map<char, std::set<char>> contrast;

    s1.push('#');
    
    std::string exp_converted = convert(regexp, contrast);
    std::cout << exp_converted << std::endl;

    for(auto &ch: exp_converted) {
        if((ch != '(') && (ch != ')') && (ch != '|') && (ch != '.') && (ch != '+') && (ch != '*')) {
            s2.push(ch);
        }
        else {
            if(ch == '(')
                s1.push(ch);
            else if(ch == ')') {
                while(s1.top() != '(') {
                    s2.push(s1.top());
                    s1.pop();
                }
                s1.pop(); //(
            }
            else {
                if(s1.top() == '(') {
                    s1.push(ch);
                }
                else {
                    while(GetPriority(s1.top()) >= GetPriority(ch)) {
                        s2.push(s1.top());
                        s1.pop();
                    }
                    s1.push(ch);
                }
            }
        }
    }

    while(s1.top() != '#') {
        s2.push(s1.top());
        s1.pop();
    }

    std::stack<char> reverse;
    while(!s2.empty()) {
        reverse.push(s2.top());
        std::cout<< s2.top();
        s2.pop();
    }
    std::cout << std::endl;

    while(!reverse.empty()) {
        reg_parsed += reverse.top();
        reverse.pop();
    }

    return contrast;
}