#include "DFA2MINIDFA.h"

int main(int argc, char *argv[])
{
    DFA2MINIDFA instance;
    instance.ReadDFA(argv[1]);
    std::set<std::set<size_t>> ans;
    ans = instance.Split();
    instance.GetMiniDFA(ans);
    instance.Show(ans);
    instance.OutputMiniDFA2File();
    return 0;
}