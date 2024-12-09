#include <queue>
#include <stack>
#include <iomanip>
#include <vector>
#include <iostream>
#include <set>
#include <map>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

struct DFA {
    set<int> states;		// 状态集
    set<char> alphabet;		// 输入字母表
    map<pair<int, char>, int> transitions;		// 状态转换函数	
    int startState;			// 开始状态
    set<int> finalStates;	// 终止状态集
    DFA(set<int> states, set<char> alphabet, map<pair<int, char>, int> transitions,
        int startState, set<int> finalStates) :states(states), alphabet(alphabet), transitions(transitions),
        startState(startState), finalStates(finalStates) {};
};

struct NFA {
    set<int> states;	// 有穷状态集
    set<char> alphabet;	// 输入字母表
    map<pair<int, char>, set<int>> transitions;	// 状态转换函数
    int startState;		// 开始状态
    set<int> finalStates;	// 终止状态集
    NFA(set<int> states, set<char> alphabet, map<pair<int, char>, set<int>> transitions,
        int startState, set<int> finalStates) :states(states), alphabet(alphabet), transitions(transitions),
        startState(startState), finalStates(finalStates) {};
    NFA() {};
};
// 打印nfa
void printNFA(NFA& nfa)
{
    cout << "状态集:";
    for (const int state : nfa.states)
    {
        cout << state << " ";
    }
    cout << endl << "输入字母表:";
    for (const char alpha : nfa.alphabet)
    {
        cout << alpha << " ";
    }
    cout << endl << "状态转换函数:" << endl;
    for (const auto& transition : nfa.transitions)
    {
        if (transition.first.second == '-')
        {
            cout << "(" << transition.first.first << ", " << -1 << ") -> ";
        }
        else
        {
            cout << "(" << transition.first.first << ", " << setw(2) << transition.first.second << ") -> ";
        }
        for (const int state : transition.second)
        {
            cout << state << " ";
        }
        cout << endl;
    }
    cout << "初始状态:" << nfa.startState << endl;
    cout << "终止状态集:";
    for (const int state : nfa.finalStates)
    {
        cout << state << " ";
    }
    cout << endl;
}

// 读取nfa
NFA readNFA(string txt_path) {
    ifstream file(txt_path); // 打开文件
    if (!file.is_open()) {
        cerr << "无法打开文件！" << endl;
        exit(1);
    }

    string line;
    int num;
    char ch;
    int startState;
    set<char> alphabet;
    set<int> states;
    set<int> finalStates;
    map<pair<int, char>, set<int>> transitions;

    // 读取状态集
    if (getline(file, line)) {
        stringstream ss(line);
        while (ss >> num) {
            states.insert(num);
        }
    }

    // 读取输入字母表
    if (getline(file, line)) {
        stringstream ss(line);
        while (ss >> ch) {
            alphabet.insert(ch);
        }
    }
    if (getline(file, line)) {
        stringstream ss(line);
        ss >> num;
        for (int i = 0; i < num; i++) {
            getline(file, line);
            if (line.empty()) break; // 遇到空行跳出循环
            stringstream ss(line);
            int fromState, toState;
            char inputChar;
            set<int> toStates;

            ss >> fromState >> inputChar;

            // 处理字符为 `'-'` 的情况
            if (inputChar == '-') {
                ss >> inputChar;
                inputChar = '-';
            }

            // 读取目标状态集合
            while (ss >> toState) {
                toStates.insert(toState);
            }

            // 将转移关系加入 `transitions`
            transitions[{fromState, inputChar}] = toStates;

        }
    }
    // 读取状态转换函数


    // 读取开始状态
    if (getline(file, line)) {
        stringstream ss(line);
        ss >> startState;
    }

    // 读取终止状态集
    if (getline(file, line)) {
        stringstream ss(line);
        while (ss >> num) {
            finalStates.insert(num);
        }
    }

    file.close(); // 关闭文件

    // 构造NFA对象并返回
    NFA nfa(states, alphabet, transitions, startState, finalStates);
    return nfa;
}

// 合并多个nfa
NFA dealNFAS(vector<NFA>& nfas)
{
    if (nfas.empty())
    {
        cout << "没有NFA可以合并！" << endl;
        exit(1);
    }
    // 初始化合并后的NFA
    NFA nfa;
    // 新的初始状态
    int newStartState = -1;
    nfa.states.insert(newStartState);
    nfa.startState = newStartState;
    for (const NFA& nfa_temp : nfas)
    {
        for (int state : nfa_temp.states)
        {
            nfa.states.insert(state);
        }

        // 合并字母表
        nfa.alphabet.insert(nfa_temp.alphabet.begin(), nfa_temp.alphabet.end());

        // 合并状态转换函数
        for (const auto& transition : nfa_temp.transitions)
        {
            int fromState = transition.first.first;
            char inputChar = transition.first.second;

            for (int toState : transition.second)
            {
                nfa.transitions[{fromState, inputChar}].insert(toState);
            }
        }

        // 合并终止状态集
        for (int finalState : nfa_temp.finalStates)
        {
            nfa.finalStates.insert(finalState);
        }

        // 添加ε转移到原始NFA的起始状态
        nfa.transitions[{newStartState, '-'}].insert(nfa_temp.startState);

    }

    return nfa;
}


set<int> epsilonClosure(const set<int>& states, const map<pair<int, char>, set<int>>& transitions);
set<int> move(const set<int>& states, char symbol, const map<pair<int, char>, set<int>>& transitions);

void printDFA(DFA& dfa)
{
    cout << "状态集:";
    for (const int state : dfa.states) {
        cout << state << " ";
    }
    cout << endl;

    cout << "输入字母表:";
    for (const char alpha : dfa.alphabet) {
        cout << alpha << " ";
    }
    cout << endl;

    cout << "状态转换函数:" << endl;
    for (const auto& transition : dfa.transitions) {
        cout << "(" << transition.first.first << ", " << transition.first.second << ") -> " << transition.second << endl;
    }

    cout << "初始状态:" << dfa.startState << endl;

    cout << "终止状态集:";
    for (const int state : dfa.finalStates) {
        cout << state << " ";
    }
    cout << endl;
}


DFA transToDFA(NFA& nfa)
{
    // 检查NFA是否已确定化
    bool oneNFA = true;
    for (const auto& transition : nfa.transitions) {
        // 如果存在环或一个状态在某输入下有多个目标状态，则不是DFA
        if (transition.first.second == '-' || transition.second.size() > 1) {
            oneNFA = false;
            break;
        }
    }

    if (oneNFA) {
        cout << endl << "NFA已确定化，直接转换为DFA" << endl;
        set<int> dfaStates = nfa.states;
        set<char> alphabet = nfa.alphabet;
        map<pair<int, char>, int> transitions;

        // 转换NFA的每个转移到DFA
        for (const auto& transition : nfa.transitions) {
            int fromState = transition.first.first;
            char symbol = transition.first.second;
            int toState = *transition.second.begin(); // 每个转移只有一个目标状态
            transitions[{fromState, symbol}] = toState;
        }

        int startState = nfa.startState;
        set<int> finalStates = nfa.finalStates;

        return DFA(dfaStates, alphabet, transitions, startState, finalStates);
    }

    // 子集构造法进行NFA到DFA的转换
    set<set<int>> dfaStates; // DFA的状态集（由NFA状态的子集组成）
    set<char> alphabet = nfa.alphabet;
    map<pair<set<int>, char>, set<int>> dfaTransitions; // DFA的转移函数

    // 计算初始状态的ε闭包
    set<int> startState = epsilonClosure({ nfa.startState }, nfa.transitions);
    dfaStates.insert(startState);

    queue<set<int>> stateQueue;
    stateQueue.push(startState);

    set<set<int>> finalStates; // DFA的终止状态集

    // 逐步处理每个状态
    while (!stateQueue.empty()) {
        set<int> currentState = stateQueue.front();
        stateQueue.pop();

        for (char symbol : alphabet) {
            // 计算当前状态集合在输入 symbol 下的目标状态集合
            set<int> nextState = move(currentState, symbol, nfa.transitions);

            if (!nextState.empty()) {
                // 对目标状态集合计算ε闭包
                nextState = epsilonClosure(nextState, nfa.transitions);

                // 如果目标状态集合是新状态，加入DFA的状态集中
                if (!dfaStates.count(nextState)) {
                    dfaStates.insert(nextState);
                    stateQueue.push(nextState);
                }

                // 更新DFA的转移函数
                dfaTransitions[{currentState, symbol}] = nextState;
            }
        }

        // 判断当前状态集合是否为终止状态
        for (int state : currentState) {
            if (nfa.finalStates.count(state)) {
                finalStates.insert(currentState);
                break;
            }
        }
    }

    // 状态集合映射为整数
    map<set<int>, int> stateMapping;
    int stateCount = 0;
    set<int> smalledStates;

    for (const auto& stateSet : dfaStates) {
        stateMapping[stateSet] = stateCount++;
        smalledStates.insert(stateMapping[stateSet]);
    }

    // 新的转移函数
    map<pair<int, char>, int> smalledTransitions;
    for (const auto& [key, value] : dfaTransitions) {
        int fromState = stateMapping[key.first];
        int toState = stateMapping[value];
        smalledTransitions[{fromState, key.second}] = toState;
    }

    // 扁平化终止状态集合
    set<int> smalledFinalStates;
    for (const auto& stateSet : finalStates) {
        smalledFinalStates.insert(stateMapping[stateSet]);
    }

    // 扁平化初始状态
    int smalledStartState = stateMapping[startState];

    // 返回确定化后的DFA
    return DFA(smalledStates, alphabet, smalledTransitions, smalledStartState, smalledFinalStates);
}

set<int> epsilonClosure(const set<int>& states, const map<pair<int, char>, set<int>>& transitions) {
    set<int> closure = states; // 初始化闭包为输入的状态集合
    stack<int> stack;          // 用于深度优先搜索的栈

    // 将所有初始状态压入栈
    for (int state : states) {
        stack.push(state);
    }

    // 深度优先搜索，遍历所有通过 ε 转移可以到达的状态
    while (!stack.empty()) {
        int currentState = stack.top();
        stack.pop();

        // 查找当前状态的 ε 转移
        auto it = transitions.find({ currentState, '-' }); // -1 表示 ε 转移
        if (it != transitions.end()) {
            for (int nextState : it->second) {
                // 如果闭包中尚未包含该状态，则加入闭包并压入栈
                if (closure.find(nextState) == closure.end()) {
                    closure.insert(nextState);
                    stack.push(nextState);
                }
            }
        }
    }

    return closure;
}


set<int> move(const set<int>& states, char symbol, const map<pair<int, char>, set<int>>& transitions) {
    set<int> result; // 存储目标状态集合

    // 遍历状态集合
    for (int state : states) {
        // 查找当前状态在指定符号下的转移
        auto it = transitions.find({ state, symbol });
        if (it != transitions.end()) {
            // 将所有目标状态加入结果集合
            result.insert(it->second.begin(), it->second.end());
        }
    }

    return result;
}

DFA mininizedDFA(DFA& dfa) {
    // 1. 初始化分组：终止状态和非终止状态
    set<int> finalStates = dfa.finalStates;
    set<int> nonFinalStates;
    bool flag = true;
    for (int state : dfa.states) {
        if (!finalStates.count(state)) {
            nonFinalStates.insert(state);
        }
    }

    // 初始划分 P：{终止状态} 和 {非终止状态}
    set<set<int>> partitions;
    partitions.insert(finalStates);
    partitions.insert(nonFinalStates);

    // 2. 开始划分过程
    while (flag) {
        flag = false;
        // 按照当前符号对分区进行划分
        set<set<int>> newPartitions;
        for (const auto& partition : partitions) {
            map<set<pair<set<int>, char>>, set<int>> veri_mp;   // 存储同一分类的set   
            for (auto state : partition) {
                set<pair<set<int>, char>> veri_set; // 存储转化状态
                for (char symbol : dfa.alphabet) {
                    if (dfa.transitions.count({ state,symbol })) {
                        int temp = dfa.transitions[{ state, symbol }];
                        for (auto partition_temp : partitions) {
                            if (partition_temp.count(temp)) {
                                veri_set.insert({ partition_temp, symbol });
                                break;
                            }
                        }
                    }
                }
                veri_mp[veri_set].insert(state);
            }
            for (auto temp : veri_mp) {
                newPartitions.insert(temp.second);
            }
        }
        // 更新分区
        if (newPartitions.size() > partitions.size()) {
            partitions = newPartitions;
            flag = true;
        }
    }

    // 3. 构造最小化 DFA
    map<set<int>, int> stateMapping; // 用于映射每个状态分组到新状态
    int newStateCounter = 0;
    for (const auto& partition : partitions) {
        stateMapping[partition] = newStateCounter++;
    }

    set<int> newStates;
    set<int> newFinalStates;
    map<pair<int, char>, int> newTransitions;
    int newStartState = -1;

    // 构造新的状态、转移函数、初始状态和终止状态
    for (const auto& partition : partitions) {
        int newState = stateMapping[partition];
        newStates.insert(newState);

        // 如果原分组包含初始状态，设置新的初始状态
        if (partition.count(dfa.startState)) {
            newStartState = newState;
        }

        // 如果分组包含终止状态，加入新的终止状态集
        for (int state : partition) {
            if (finalStates.count(state)) {
                newFinalStates.insert(newState);
                break;
            }
        }

        // 构造转移函数
        for (int state : partition) {
            for (char symbol : dfa.alphabet) {
                auto it = dfa.transitions.find({ state, symbol });
                if (it != dfa.transitions.end()) {
                    int targetState = it->second;

                    // 找到目标状态所属的分组
                    for (const auto& targetPartition : partitions) {
                        if (targetPartition.count(targetState)) {
                            newTransitions[{newState, symbol}] = stateMapping[targetPartition];
                            break;
                        }
                    }
                }
            }
        }
    }

    // 返回最小化 DFA
    return DFA(newStates, dfa.alphabet, newTransitions, newStartState, newFinalStates);
}

const string TXT_PATH = "D:\\study\\专业课\\编译原理\\Lab2\\NFA1.txt";

int main(int argc, char* argv[]) {
    vector<NFA> nfas;

    if (argc < 2)
    {
        cout << "未检测到参数，读取默认txt：" << TXT_PATH << endl;
        string s = TXT_PATH;
        NFA nfa = readNFA(s);
        nfas.push_back(nfa);
    }
    else {
        // 逐个读取命令行中的文件，构造NFA
        for (int i = 1; i < argc; i++)
        {
            string s = argv[i];
            NFA nfa = readNFA(s);
            nfas.push_back(nfa);
        }
    }
    NFA nfa = dealNFAS(nfas);
    cout << "合并后的NFA" << endl;
    printNFA(nfa);

    DFA dfa = transToDFA(nfa);
    cout << endl << "NFA转成DFA" << endl;
    printDFA(dfa);

    dfa = mininizedDFA(dfa);
    cout << endl << "合并后的DFA" << endl;
    printDFA(dfa);


    return 0;
}