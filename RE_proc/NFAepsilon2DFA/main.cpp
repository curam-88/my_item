#include"NFAepsilon2DFA.h"
int main()
{
	 NFAepsilon2DFA a;
	 std::string file_name = "NFAepsilon.txt";
	 a.ReadNFAepsilon(file_name);
	 a.SetNFA();
	 //cout<<a.Q;
	 //cout<<"\n";
	 return 0;
}