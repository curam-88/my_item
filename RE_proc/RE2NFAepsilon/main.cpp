#include "RE.h"
#include "NFAepsilon.h"

int main()
{
    std::string regexp = "(([a~c].[A~D])*.([0~2]*.[A~B]|[a~e])*)+";
    //std::string regexp = "[a~c]+";
    std::string reg_parsed("\0");
    std::map<char, std::set<char>> contrast;
    RE RE_proc(regexp);
    contrast = RE_proc.Parse(regexp, reg_parsed);

    NFAepsilon NFAepsilon_proc(contrast);
    Unit NFA_epsilon = NFAepsilon_proc.Reg2NFAepsilon(reg_parsed);
    NFAepsilon_proc.Display(NFA_epsilon);
    NFAepsilon_proc.NFAepsilonSummary(NFA_epsilon);
    NFAepsilon_proc.Output("NFAepsilon.txt");
    return 0;
}