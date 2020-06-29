#include "./RE2NFAepsilon/RE.h"
#include "./RE2NFAepsilon/NFAepsilon.h"
#include "./NFAepsilon2DFA/NFAepsilon2DFA.h"
#include "./DFA2MINIDFA/DFA2MINIDFA.h"

int main(int argc, char *argv[])
{
    //std::string regexp = "(([a~c].[A~D])*.([0~2]*.[A~B]|[a~e])*)+";
    if(argc != 2) {
        std::cout << "Usage: ./a.out <RegularExpression>" << std::endl;
        exit(0);
    }
    std::string regexp = argv[1];
    std::string::iterator it;
    for(it = regexp.begin(); it < regexp.end();it++)
    {
        if(*it == '"')
        {
            regexp.erase(it);
        }
        it--;
    }
    std::string reg_parsed("\0");
    std::map<char, std::set<char>> contrast;
    RE RE_proc(regexp);
    contrast = RE_proc.Parse(regexp, reg_parsed);

    NFAepsilon NFAepsilon_proc(contrast);
    Unit NFA_epsilon = NFAepsilon_proc.Reg2NFAepsilon(reg_parsed);
    NFAepsilon_proc.Display(NFA_epsilon);
    NFAepsilon_proc.NFAepsilonSummary(NFA_epsilon);
    NFAepsilon_proc.Output("NFAepsilon.txt");

    NFAepsilon2DFA NFA2DFA_proc;
	std::string file_name = "NFAepsilon.txt";
	NFA2DFA_proc.ReadNFAepsilon(file_name);
	NFA2DFA_proc.SetNFA();

    std::string dfa_file = "DFA.txt";
    DFA2MINIDFA NFA2MINIDFA_proc;
    NFA2MINIDFA_proc.ReadDFA(dfa_file);
    std::set<std::set<size_t>> ans;
    ans = NFA2MINIDFA_proc.Split();
    NFA2MINIDFA_proc.GetMiniDFA(ans);
    NFA2MINIDFA_proc.Show(ans);
    NFA2MINIDFA_proc.OutputMiniDFA2File();
    return 0;
}
