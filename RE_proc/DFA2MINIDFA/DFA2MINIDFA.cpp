#include "DFA2MINIDFA.h"
DFA2MINIDFA::DFA2MINIDFA(){}

DFA2MINIDFA::~DFA2MINIDFA(){}

void DFA2MINIDFA::ReadDFA(std::string filename)
{
    std::ifstream fp;
	fp.open(filename);
    if(!fp.is_open()) {
        std::cout << filename << " open error." << std::endl;
        exit(0);
    }

	std::string tempstr;
	while(getline(fp, tempstr)) {
		if(tempstr == "Q: ") {
			std::string stateline;
			getline(fp, stateline);
			std::string state;
			for(size_t i = 0; i < stateline.size(); i++) {
				if(stateline[i]==' ') {
					size_t res = atoi(state.c_str());
					this->Q.insert(res);
					state.clear();
				}
				else
					state += stateline[i];
			} 
		}
        else if(tempstr == "Sigma: ") {
			std::string sigmaline;
			getline(fp, sigmaline);
			for(size_t i = 0; i < sigmaline.size(); i++) {
				if(sigmaline[i] == ' ')
					continue;
				else
					this->Sigma.insert(sigmaline[i]);
			} 
		}
		else if(tempstr == "q0: ") {
			std::string q0line;
			getline(fp, q0line);
			size_t res = atoi(q0line.c_str());
			this->Q0 = res;
		}
		else if(tempstr == "F: ") {
			std::string fline;
			getline(fp, fline);
			std::string state;
			for(size_t i = 0; i < fline.size(); i++) {
				if(fline[i]==' ') {
					size_t res = atoi(state.c_str());
					this->F.insert(res);
					state.clear();
				}
				else
					state += fline[i];
			} 
		}
		else if(tempstr == "Delta: ") {
			std::string deltaline;
			while(!fp.eof())
			{
				deltaline.clear();
				getline(fp, deltaline);
                if(deltaline[0] != '(')
                    break;
				size_t index(1);
				std::string qBegin;
				char transchar;
				std::string qEnd;
				while(deltaline[index] != ',') { //提取起始状态
					qBegin += deltaline[index];
					index++;
				}
				index += 2;  //,{
				transchar = deltaline[index];
				index += 7;

				for(size_t i = index; i < deltaline.size(); i++)
				{
					qEnd += deltaline[i];
				}

				size_t _qBegin = atoi(qBegin.c_str());
				size_t _qEnd = atoi(qEnd.c_str());
				std::pair<size_t, char> left(_qBegin, transchar);
                this->Delta.insert(std::pair<std::pair<size_t, char>, size_t>(left, _qEnd));
			}
			break;
		}
		tempstr.clear();
	}
    fp.close();
}

size_t DFA2MINIDFA::Move(size_t qBegin, char transchar)
{
    std::pair<size_t, char> key(qBegin, transchar);
    if(this->Delta.find(key) != this->Delta.end())
        return Delta[key];
    return qBegin;
}

std::set<size_t> DFA2MINIDFA::SetCount(std::map<size_t, size_t> state2set)
{
    std::set<size_t> res;
    for(auto elem: state2set) {
        res.insert(elem.second);
    }
    return res;
}

std::set<std::set<size_t>> DFA2MINIDFA::Split()
{
    size_t set_index(0);
    std::map<std::set<size_t>, size_t> indexedSet;
    std::set<size_t> accept = this->F;
    std::set<size_t> nonaccept = this->Q;
    
    for(auto elem: this->F) {
        nonaccept.erase(elem);
    }

    indexedSet.insert(std::pair<std::set<size_t>, size_t>(accept, set_index++));
    indexedSet.insert(std::pair<std::set<size_t>, size_t>(nonaccept, set_index++));

    std::set<std::set<size_t>> Pai_initial;
    Pai_initial.insert(accept);
    Pai_initial.insert(nonaccept);

    bool isSplited(false);
    bool isStable(false);
    std::map<size_t, size_t> state2setindex;
    while(!isStable) {
        size_t counter(0);
        for(auto &subset: Pai_initial) {
            counter++;
            if(subset.size() > 1) {
                for(auto transchar: this->Sigma) {
                    state2setindex.clear();
                    for(auto state: subset) {
                        size_t qEnd = Move(state, transchar);
                        size_t index;
                        for(auto mapElem: indexedSet) {
                            if(mapElem.first.find(qEnd) != mapElem.first.end()) {
                                index = mapElem.second;
                                break;
                            }
                        }
                        state2setindex.insert(std::pair<size_t, size_t>(state, index));
                    }

                    std::set<size_t> setOfindex = SetCount(state2setindex);
                    if(setOfindex.size() != 1) {
                        isSplited = true;

                        std::map<std::set<size_t>, size_t>::iterator it = indexedSet.find(subset);
                        for(auto a: indexedSet) {
                            std::cout << "Set elem:";
                            for(auto b: a.first) {
                                std::cout << b << ' ';
                            }
                            std::cout << std::endl;
                            std::cout << "second:" << a.second;
                            std::cout << std::endl;
                        }
                        if(it != indexedSet.end())
                            indexedSet.erase(it);
                        //indexedSet.erase(subset);
                        Pai_initial.erase(subset);

                        for(auto _index: setOfindex) {
                            std::set<size_t> newSubset;
                            for(auto elem: state2setindex) {
                                if(elem.second == _index) {
                                    newSubset.insert(elem.first);
                                }
                            }
                            Pai_initial.insert(newSubset);
                            indexedSet.insert(std::pair<std::set<size_t>, size_t>(newSubset, set_index++));
                        }
                        break;
                    }
                    else
                        continue;
                }
                if(isSplited) {
                    isSplited = false;
                    break;
                }
            }
            
        }
        if(counter == Pai_initial.size())
            isStable = true;
    }
    return Pai_initial;
}

void DFA2MINIDFA::Show(std::set<std::set<size_t>> Pai_initial)
{
    for(auto elem: Pai_initial) {
        for(auto state: elem) {
            std::cout << state << ' ';
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
    for(auto elem: this->minDelta) {
        std::cout << elem.first.first << ',' << elem.first.second << "->" << elem.second << std::endl;
    }

}

size_t ReplaceState(std::map<std::set<size_t>, size_t> contrast, size_t state)
{
    size_t res = state;
    for(auto elem: contrast) {
        if(elem.first.find(state) != elem.first.end()) {
            res = elem.second;
            break;
        }
    }
    return res;
}

void DFA2MINIDFA::GetMiniDFA(std::set<std::set<size_t>> Pai_initial)
{
    std::map<std::set<size_t>, size_t> contrast;
    for(auto elem: Pai_initial) {
        contrast.insert(std::pair<std::set<size_t>, size_t>(elem, *elem.begin()));
    }
    for(auto elem: this->Delta) {
        std::pair<std::pair<size_t, char>, size_t> ans;
        ans.first.first = ReplaceState(contrast, elem.first.first);
        ans.first.second = elem.first.second;
        ans.second = ReplaceState(contrast, elem.second);
        std::multimap<std::pair<size_t, char>, size_t>::iterator minit = this->minDelta.find(ans.first);
        if(minit != this->minDelta.end()) {
            if(minit->second == ans.second) {
                continue;
            }
            else
                this->minDelta.insert(ans);
        }
        else
            this->minDelta.insert(ans);
    }

    this->minQ0 = ReplaceState(contrast, this->Q0);

    for(auto elem: this->Q) {
        this->minQ.insert(ReplaceState(contrast, elem));
    }

    for(auto elem: this->F) {
        this->minF.insert(ReplaceState(contrast, elem));
    }

    this->minSigma = this->Sigma;
}

void DFA2MINIDFA::OutputMiniDFA2File()
{
    std::ofstream fp;
    fp.open("MiniDFA.txt");
    if(!fp.is_open()){
        std::cout << "Cannot open DFA.txt." << std::endl;
        exit(0);
    }

    fp << "Q: " << '\n';
    for(auto &qmem:this->minQ) {
        fp << qmem << ' ';
    }
    fp << '\n';

    fp << "Sigma: " << '\n';
    for(auto &sigmem:this->minSigma) {
        // if(sigmem == 0)
        //     fp << "\\0" << ' ';
        // else
            fp << sigmem << ' ';
    }
    fp << '\n';

    fp << "q0: " << '\n';
    fp << this->minQ0 << '\n';

    fp << "F: " << '\n';
    for(auto &fmem:this->minF) {
        fp << fmem << ' ';
    }
    fp << '\n';

    fp << "Delta: " << '\n';
    for(auto &fmem:this->minDelta) {
        fp << '(' << fmem.first.first << ',' << fmem.first.second << ')' << " -> " << fmem.second << '\n';
    }

    fp.close();
}