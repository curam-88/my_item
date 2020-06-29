#include "NFAepsilon2DFA.h"


NFAepsilon2DFA::NFAepsilon2DFA(){}
NFAepsilon2DFA::~NFAepsilon2DFA(){}
void NFAepsilon2DFA::ReadNFAepsilon(std::string file_name)
{
	std::ifstream fp;
	fp.open(file_name);
	std::string str;
	while(getline(fp,str))
	{
		if(str == "Q: ")
		{
			std::string state;
			getline(fp,state);
			std::string q;
			for(int i=0;i<state.size();i++)
			{
				if(state[i]==' ')
				{
					size_t num = atoi(q.c_str());
					this->Q.insert(num);
					q.clear();
				}
				else
					q += state[i];
			} 
		}
		else if(str == "Sigma: ")
		{
			std::string state;
			getline(fp,state);
			for(int i=0;i<state.size();i++)
			{
				if(state[i] == ' ')
				{
					continue;
				}
				else
					this->Sigma.insert(state[i]);
			} 
		}
		else if(str == "q0: ")
		{
			std::string state;
			getline(fp,state);
			size_t num = atoi(state.c_str());
			this->Q0 =  num;
		}
		else if(str == "F: ")
		{
			std::string state;
			getline(fp,state);
			size_t num = atoi(state.c_str());
			this->F =  num;
		}
		else if(str == "Delta: ")
		{
			std::string state;
			while(!fp.eof())
			{
				state.clear();
				getline(fp,state);
				if(state[0]!='(')
					break;
				int index = 1;
				std::string Q0;
				std::set<char> detal;
				std::string F;
				while(state[index]!=',')//提取起始状态
				{
					Q0 += state[index];
					index++;
				}
				index+=2;
				while(state[index]!='}')
				{
					if(state[index]!=',')
					{
						detal.insert(state[index]);
					}
					index++;
				}
				index +=6;
				for(int i=index;i<state.size();i++)
				{
					F+=state[i];
				}
				//std::cout<<state<<"\n";
				size_t num = atoi(Q0.c_str());
				size_t f = atoi(F.c_str());
				std::pair<size_t, std::set<char>> left(num,detal);
				this->Delta.insert(std::pair<std::pair<size_t, std::set<char>>, size_t>(left, f));	
			}
			break;
		}
		else
		{
			std::cout << "the file_name error!\n";
		}
		str.clear();
	}
}
void NFAepsilon2DFA::SetNFA()
{
	size_t state = 0;//映射表的数目
	size_t detal_size = 0;//转换关系的数目
	this->Q_to[state].new_state = state;
	std::set<size_t> BEGIN_Q={this->Q0};
	this->Q_to[state].old_state = DeleteAepsilon(BEGIN_Q);
	//for(auto ppp : this->Q_to[state].old_state)
		//std::cout<<ppp;
	state++;
	for(int i=0;i<state;i++)//遍历所有映射表
	{
		for(auto word:this->Sigma)
		{
			if(word=='#')
				continue;
			std::set<size_t> old_Q = DeleteAepsilon(InWord(this->Q_to[i].old_state,word));//统计添加该字符产生的状态集合
			if(old_Q.empty())//不存在结果集合继续执行
			{
				continue;
			}
			int judge = 0;
			int endstate = -1;
			for(int j=0;j<state;j++)//判断是否存在该状态
			{
				if(EqualSet(this->Q_to[j].old_state,old_Q))
				{
					judge = 1;
					endstate = j;
					break;
				}
			}
			if(judge)//存在该状态
			{
				int j = 0;
				//若转NFA
				/*for(;j<detal_size;j++)//若存在该转换函数添加字符
				{
					if(this->detal[j].start==this->Q_to[i].state&&this->detal[j].end==endstate)
					{
						this->detal[j].change.insert(word);
						judge = 0;
					}
				}
				//if(judge)//不存在该转换函数(添加转换表)
				//{*/
				this->detal[detal_size].start = this->Q_to[i].new_state;
				this->detal[detal_size].change.insert(word);
				this->detal[detal_size].end = this->Q_to[endstate].new_state;
				detal_size++;//转换函数数目加一
				//}
			}
			else//不存在该状态（既添加转换函数又要添加映射表）
			{
				this->Q_to[state].new_state = state;
				this->Q_to[state].old_state = old_Q;
				state++;//状态映射数目加一
				this->detal[detal_size].start = this->Q_to[i].new_state;
				this->detal[detal_size].change.insert(word);
				this->detal[detal_size].end = this->Q_to[state-1].new_state;
				detal_size++;//转换函数数目加一
			}
		}
	}
	OutPut(state,detal_size);
	FILEOUT(state,detal_size);
}
bool NFAepsilon2DFA::EqualSet(std::set<size_t> Q1,std::set<size_t> Q2)//比较2个set是否相同
{
	if(Q1.size() != Q2.size())
		return false;
	std::set<size_t>::iterator it1 = Q1.begin();
	std::set<size_t>::iterator it2 = Q2.begin();
	if(it1==Q1.end()&&it2 == Q2.end())
		return true;
	for(;it1!=Q1.end()&&it2 != Q2.end();it1++,it2++)
	{
		if(*it1 != *it2)
			return false;
	}
	return true;

}
std::set<size_t> NFAepsilon2DFA::DeleteAepsilon(std::set<size_t> Q_all)//处理#字符
{
	std::set<size_t> res;
	res = Q_all;
	for(auto &p : Q_all)
	{
		Eclouse(p,res);
	}
	return res;
}
void NFAepsilon2DFA::Eclouse(size_t c,std::set<size_t> &res)//检索该字符的所有跳空字符
{
	for(auto &p : this->Delta)
	{
		if(p.first.first==c&&PrintSet(p.first.second) == "#")
		{	
			if(res.find(p.second) != res.end())
				continue;
			else
			{
			res.insert(p.second);
			Eclouse(p.second,res);
			}
		}
	}
	return;
}
std::string NFAepsilon2DFA::PrintSet(std::set<char> charset)//set变成字符串
{
    std::string res;
    std::set<char>::iterator it = charset.begin();
    for( ; it != charset.end(); it++)
        res += *it;
    return res;
}
std::string NFAepsilon2DFA::FilePrintSet(std::set<char> charset)//set变成字符串
{
    std::string res;
    std::set<char>::iterator it = charset.begin();
    std::set<char>::iterator ip = charset.begin();
    for( ; it != charset.end(); it++)
    {   
     	res += *it;
     	ip = it;
     	if(++ip!=charset.end())
     		res += ',';
    }
    return res;
}
std::set<size_t> NFAepsilon2DFA::InWord(std::set<size_t> qq,char word)//根据输入的符号确定新的状态
{
	std::set<size_t> res;
	for(auto &q : qq)
	{	
		for(auto &p : this->Delta)
		{
			if(p.first.first == q)//存在到达的字符添加状态
			{
				if(p.first.second.find(word)!=p.first.second.end())
					res.insert(p.second);
			}
		}
	}
	return res;
}
void NFAepsilon2DFA::OutPut(int state_size,int detal_size)
{
	std::cout<<"映射关系:"<<"\n";
	for(int i=0;i<state_size;i++)
	{
		std::cout << "新状态：" << this->Q_to[i].new_state << "\n";
		std::cout << "表示的状态集合:"<< "\n";
		for(auto &state:this->Q_to[i].old_state)
		{
		std::cout << state <<" ";
		if(state == this->F)//存在结束结点
			this->Ft.insert(i);
		if(state == this->Q0)//存在开始结点
			this->Q0t.insert(i);
		}
		std::cout<<"\n";
	}
	std::cout << "起始状态:";
	for(auto start:this->Q0t)
    std::cout << start <<" ";
	std::cout << '\n';
	std::cout<<"状态跳转关系:\n";
	for(int i=0;i<detal_size;i++)
	{
		std::cout << "第 "<<i+1<<"条边:" << this->detal[i].start << " 过程:{" << this->PrintSet(detal[i].change) << "} 结果: " << detal[i].end << "\n";
	}
	std::cout << "结束状态: \n";
	for(auto state:this->Ft)
		std::cout << state << " ";
	std::cout << "\n";
}
void NFAepsilon2DFA::FILEOUT(int state_size,int detal_size)
{
	std::ofstream fp;
    fp.open("DFA.txt");
    if(!fp.is_open()){
        std::cout << "Cannot open DFA.txt." << std::endl;
        exit(0);
    }

    fp << "Q: " << '\n';
    for(int i=0;i<state_size;i++){
        fp << this->Q_to[i].new_state << ' ';
    }
    fp << '\n';
    fp << "Sigma: " << '\n';
    for(auto &sigmem: this->Sigma){
        if(sigmem == 0)
            fp << "\\0" << ' ';
        else if(sigmem == '#')
        	continue;
        else
            fp << sigmem << ' ';
    }
    fp << '\n';
    fp << "q0: " << '\n';
    for(auto start:this->Q0t)
    fp << start;
	fp << '\n';
    fp << "F: " << '\n';
    for(auto end:this->Ft)
    fp << end << ' ';
	fp<<'\n';
    fp << "Delta: " << '\n';
    for(int i=0;i<detal_size;i++) {
        fp << '(' << this->detal[i].start << ",{" << FilePrintSet(this->detal[i].change) << "})" << " -> " << this->detal[i].end << '\n';
    }
    fp.close();
}