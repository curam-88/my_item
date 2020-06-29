#include "NFAepsilon.h"

NFAepsilon::NFAepsilon(std::map<char, std::set<char>> contrast):StateCount(0)
{
    this->contrast = contrast;
}

NFAepsilon::~NFAepsilon(){}

Unit NFAepsilon::Reg2NFAepsilon(std::string rpn)
{
    Unit left, right, unit_res;
    std::stack<Unit> polish_handle;

    for(auto ch:rpn) {
        switch (ch)
        {
        case '|':
            right = polish_handle.top();
            polish_handle.pop();
            left = polish_handle.top();
            polish_handle.pop();
            unit_res = Union(left, right);
            polish_handle.push(unit_res);
            break;
        case '*':
            left = polish_handle.top();
			polish_handle.pop();
			unit_res = Kleene1(left);
			polish_handle.push(unit_res);
			break;
        case '+':
            left = polish_handle.top();
			polish_handle.pop();
			unit_res = Kleene2(left);
			polish_handle.push(unit_res);
			break;
        case '.':
            right = polish_handle.top();
            polish_handle.pop();
            left = polish_handle.top();
            polish_handle.pop();
            unit_res = Join(left, right);
            polish_handle.push(unit_res);
            break;
        default:
            unit_res = MakeUnit(ch);
            polish_handle.push(unit_res);
            break;
        }
    }
    unit_res = polish_handle.top();
    polish_handle.pop();  //make clear
    return unit_res;
}

Unit NFAepsilon::Union(Unit left, Unit right)
{
    Unit unit_res;
	unit_res.EdgeCount = 0;

	Edge edge1, edge2, edge3, edge4;

	//获得新的新状态节点
	size_t StartState = NewState();
	size_t EndState = NewState();

	//构建边
	edge1.StartState = StartState;
	edge1.EndState = left.StartState;
	edge1.CharSet = this->epsilon;

	edge2.StartState = StartState;
	edge2.EndState = right.StartState;
	edge2.CharSet = this->epsilon;

	edge3.StartState = left.EndState;
	edge3.EndState = EndState;
	edge3.CharSet = this->epsilon;

	edge4.StartState = right.EndState;
	edge4.EndState = EndState;
	edge4.CharSet = this->epsilon;


	//构建单元
	//先将Left和Right的EdgeSet复制到unit_res
	AddEdge2UnitFromUnit(unit_res,left);
	AddEdge2UnitFromUnit(unit_res,right);

	//将新构建的四条边加入EdgeSet
	unit_res.EdgeSet[unit_res.EdgeCount++] = edge1;
	unit_res.EdgeSet[unit_res.EdgeCount++] = edge2;
	unit_res.EdgeSet[unit_res.EdgeCount++] = edge3;
	unit_res.EdgeSet[unit_res.EdgeCount++] = edge4;

	//构建unit_res的起始状态和结束状态
	unit_res.StartState = StartState;
	unit_res.EndState = EndState;

	return unit_res;
}

Unit NFAepsilon::Kleene1(Unit left)
{
    Unit unit_res;
	unit_res.EdgeCount = 0;
	Edge edge1, edge2, edge3, edge4;

	//获得新的新状态节点
	size_t StartState = NewState();
	size_t EndState = NewState();

	//构建边
	edge1.StartState = StartState;
	edge1.EndState = EndState;
	edge1.CharSet = this->epsilon;

	edge2.StartState = left.EndState;
	edge2.EndState = left.StartState;
	edge2.CharSet = this->epsilon;

	edge3.StartState = StartState;
	edge3.EndState = left.StartState;
	edge3.CharSet = this->epsilon;

	edge4.StartState = left.EndState;
	edge4.EndState = EndState;
	edge4.CharSet = this->epsilon;
	
	//构建单元
	//先将left的EdgeSet复制到unit_res
	AddEdge2UnitFromUnit(unit_res, left);
	//将新构建的四条边加入EdgeSet
	unit_res.EdgeSet[unit_res.EdgeCount++] = edge1;
	unit_res.EdgeSet[unit_res.EdgeCount++] = edge2;
	unit_res.EdgeSet[unit_res.EdgeCount++] = edge3;
	unit_res.EdgeSet[unit_res.EdgeCount++] = edge4;
	//构建unit_res的起始状态和结束状态
	unit_res.StartState = StartState;
	unit_res.EndState = EndState;	

	return unit_res;
}

Unit NFAepsilon::Kleene2(Unit left)
{
    Unit unit_res;
	unit_res.EdgeCount = 0;
	Edge edge2, edge3, edge4;

	//获得新的新状态节点
	size_t StartState = NewState();
	size_t EndState = NewState();

	//构建边

	edge2.StartState = left.EndState;
	edge2.EndState = left.StartState;
	edge2.CharSet = this->epsilon;

	edge3.StartState = StartState;
	edge3.EndState = left.StartState;
	edge3.CharSet = this->epsilon;

	edge4.StartState = left.EndState;
	edge4.EndState = EndState;
	edge4.CharSet = this->epsilon;
	
	//构建单元
	//先将left的EdgeSet复制到unit_res
	AddEdge2UnitFromUnit(unit_res, left);
	unit_res.EdgeSet[unit_res.EdgeCount++] = edge2;
	unit_res.EdgeSet[unit_res.EdgeCount++] = edge3;
	unit_res.EdgeSet[unit_res.EdgeCount++] = edge4;
	//构建unit_res的起始状态和结束状态
	unit_res.StartState = StartState;
	unit_res.EndState = EndState;	

	return unit_res;
}

Unit NFAepsilon::Join(Unit left, Unit right) 
{
    //将Left的结束状态和Right的开始状态合并，将Right的边复制给Left，将Left返回
	//将Right中所有以StartState开头的边全部修改，
	for(size_t i = 0; i < right.EdgeCount; i++) {
		if(right.EdgeSet[i].StartState == right.StartState) {  //边 节点
			right.EdgeSet[i].StartState = left.EndState;
			//this->StateCount--;
		}
		else if(right.EdgeSet[i].EndState == right.StartState) {
			right.EdgeSet[i].EndState = left.EndState;
			//this->StateCount--;
		}
	}
	right.StartState = left.EndState;
	
	AddEdge2UnitFromUnit(left, right);
	//将Left的结束状态更新为Right的结束状态
	left.EndState = right.EndState;

	return left;
}

Unit NFAepsilon::MakeUnit(char ch)
{
    Unit unit_res;
	unit_res.EdgeCount = 0;
	Edge edge;
	//获得新的新状态节点
	size_t StartState = NewState();
	size_t EndState = NewState();
	//构建边
	edge.StartState = StartState;
	edge.EndState = EndState;
	edge.CharSet = GetSetFromMap(ch);
	//构建单元
	unit_res.EdgeSet[unit_res.EdgeCount++] = edge;
	unit_res.StartState = unit_res.EdgeSet[0].StartState;
	unit_res.EndState = unit_res.EdgeSet[0].EndState;//EdgeCount此时为1

	return unit_res;
}

size_t NFAepsilon::NewState()
{
    size_t res = this->StateCount;
    this->StateCount++;
    return res;
}

void NFAepsilon::AddEdge2UnitFromUnit(Unit& dest, Unit src)
{
    size_t d_count = dest.EdgeCount;
    size_t s_count = src.EdgeCount;

    for(size_t i = 0; i < s_count; i++) {
		dest.EdgeSet[d_count + i] = src.EdgeSet[i];
	}

	dest.EdgeCount = d_count + s_count;
}

std::string NFAepsilon::PrintSet(std::set<char> charset)
{
    std::string res("{");
    //std::cout << '{';
    res += *charset.begin();
    std::set<char>::iterator it = ++charset.begin();
    for( ; it != charset.end(); it++) {
        res += ',';
        res += *it;
    }
    res += '}';
    return res;
    // for(auto ch: charset) {

    //     std::cout << ch << ' ';
    // }
    // std::cout << '}' << std::endl;

}

void NFAepsilon::Display(Unit unit)
{
    std::cout << "NFA 的边数：" << unit.EdgeCount << std::endl;
	std::cout << "NFA 的起始状态：" << unit.StartState << std::endl;
	std::cout << "NFA 的结束状态：" << unit.EndState << std::endl;

	for(size_t i = 0; i < unit.EdgeCount; i++) {
		std::cout << "第" << i + 1 << "条边的起始状态：" << unit.EdgeSet[i].StartState
			<< "  结束状态：" << unit.EdgeSet[i].EndState
			<< "  转换符：" << PrintSet(unit.EdgeSet[i].CharSet) << std::endl;
	}
	std::cout << "结束" << std::endl;
}

std::set<char> NFAepsilon::GetSetFromMap(char ch)
{
    if(this->contrast.find(ch) != this->contrast.end()) {
        return this->contrast[ch];
    }
    std::set<char> emptyset;
    emptyset.clear();
    return emptyset;
}

void NFAepsilon::NFAepsilonSummary(Unit unit)
{
    for(size_t i = 0; i < unit.EdgeCount; i++) {
        this->Q.insert(unit.EdgeSet[i].StartState);
        this->Q.insert(unit.EdgeSet[i].EndState);

        this->Sigma.insert(unit.EdgeSet[i].CharSet.begin(), unit.EdgeSet[i].CharSet.end());

        this->Q0 = unit.StartState;
        this->F = unit.EndState;

        std::pair<size_t, std::set<char>> Delta_left(unit.EdgeSet[i].StartState, unit.EdgeSet[i].CharSet);
        this->Delta.insert(std::pair<std::pair<size_t, std::set<char>>, size_t>(Delta_left, unit.EdgeSet[i].EndState));
    }
}


void NFAepsilon::Output(std::string filename)
{
    std::ofstream fp;
    fp.open(filename);
    if(!fp.is_open()){
        std::cout << "Cannot open NFAepsilon.txt." << std::endl;
        exit(0);
    }

    fp << "Q: " << '\n';
    for(auto &qmem: this->Q) {
        fp << qmem << ' ';
    }
    fp << '\n';

    fp << "Sigma: " << '\n';
    for(auto &sigmem: this->Sigma) {
        if(sigmem == 0)
            fp << "\\0" << ' ';
        else
            fp << sigmem << ' ';
    }
    fp << '\n';

    fp << "q0: " << '\n';
    fp << this->Q0 << '\n';

    fp << "F: " << '\n';
    fp << this->F << '\n';

    fp << "Delta: " << '\n';
    for(auto &fmem:this->Delta) {
        fp << '(' << fmem.first.first << ',' << PrintSet(fmem.first.second) << ')' << " -> " << fmem.second << '\n';
    }

    fp.close();
}