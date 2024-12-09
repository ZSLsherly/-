//#include "help.h"
//#include <iomanip>
//
//// 打印nfa
//void printNFA(NFA& nfa)
//{
//    cout << "状态集:";
//    for (const int state : nfa.states)
//    {
//        cout << state << " ";
//    }
//    cout << endl << "输入字母表:";
//    for (const char alpha : nfa.alphabet)
//    {
//        cout << alpha << " ";
//    }
//    cout << endl << "状态转换函数:" << endl;
//    for (const auto& transition : nfa.transitions)
//    {
//        if (transition.first.second == '-')
//        {
//            cout << "(" << transition.first.first << ", " << -1 << ") -> ";
//        }
//        else
//        {
//            cout << "(" << transition.first.first << ", " <<setw(2)<< transition.first.second << ") -> ";
//        }
//        for (const int state : transition.second)
//        {
//            cout << state << " ";
//        }
//        cout << endl;
//    }
//    cout << "初始状态:" << nfa.startState << endl;
//    cout << "终止状态集:";
//    for (const int state : nfa.finalStates)
//    {
//        cout << state << " ";
//    }
//    cout << endl;
//}
//
//// 读取nfa
//NFA readNFA(string txt_path) {
//    ifstream file(txt_path); // 打开文件
//    if (!file.is_open()) {
//        cerr << "无法打开文件！" << endl;
//        exit(1);
//    }
//
//    string line;
//    int num;
//    char ch;
//    int startState;
//    set<char> alphabet;
//    set<int> states;
//    set<int> finalStates;
//    map<pair<int, char>, set<int>> transitions;
//
//    // 读取状态集
//    if (getline(file, line)) {
//        stringstream ss(line);
//        while (ss >> num) {
//            states.insert(num);
//        }
//    }
//
//    // 读取输入字母表
//    if (getline(file, line)) {
//        stringstream ss(line);
//        while (ss >> ch) {
//            alphabet.insert(ch);
//        }
//    }
//    if (getline(file, line)) {
//        stringstream ss(line);
//        ss >> num;
//        for (int i = 0; i < num; i++) {
//            getline(file, line);
//            if (line.empty()) break; // 遇到空行跳出循环
//            stringstream ss(line);
//            int fromState, toState;
//            char inputChar;
//            set<int> toStates;
//
//            ss >> fromState >> inputChar;
//
//            // 处理字符为 `'-'` 的情况
//            if (inputChar == '-') {
//                ss >> inputChar;
//                inputChar = '-';
//            }
//
//            // 读取目标状态集合
//            while (ss >> toState) {
//                toStates.insert(toState);
//            }
//
//            // 将转移关系加入 `transitions`
//            transitions[{fromState, inputChar}] = toStates;
//
//        }
//    }
//    // 读取状态转换函数
//
//
//    // 读取开始状态
//    if (getline(file, line)) {
//        stringstream ss(line);
//        ss >> startState;
//    }
//
//    // 读取终止状态集
//    if (getline(file, line)) {
//        stringstream ss(line);
//        while (ss >> num) {
//            finalStates.insert(num);
//        }
//    }
//
//    file.close(); // 关闭文件
//
//    // 构造NFA对象并返回
//    NFA nfa(states, alphabet, transitions, startState, finalStates);
//    return nfa;
//}
//
//// 合并多个nfa
//NFA dealNFAS(vector<NFA>& nfas)
//{
//    if (nfas.empty())
//    {
//        cout << "没有NFA可以合并！" << endl;
//        exit(1);
//    }
//    // 初始化合并后的NFA
//    NFA nfa;
//    // 新的初始状态
//    int newStartState = -1;
//    nfa.states.insert(newStartState);
//    nfa.startState = newStartState;
//    for (const NFA& nfa_temp : nfas)
//    {
//        for (int state : nfa_temp.states)
//        {
//            nfa.states.insert(state);
//        }
//
//        // 合并字母表
//        nfa.alphabet.insert(nfa_temp.alphabet.begin(), nfa_temp.alphabet.end());
//
//        // 合并状态转换函数
//        for (const auto& transition : nfa_temp.transitions)
//        {
//            int fromState = transition.first.first;
//            char inputChar = transition.first.second;
//
//            for (int toState : transition.second)
//            {
//                nfa.transitions[{fromState, inputChar}].insert(toState);
//            }
//        }
//
//        // 合并终止状态集
//        for (int finalState : nfa_temp.finalStates)
//        {
//            nfa.finalStates.insert(finalState);
//        }
//
//        // 添加ε转移到原始NFA的起始状态
//        nfa.transitions[{newStartState, '-'}].insert(nfa_temp.startState);
//
//    }
//
//    return nfa;
//}
