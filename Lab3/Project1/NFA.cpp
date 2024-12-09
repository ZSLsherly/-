#include "help.h"
#include <iomanip>

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
            cout << "(" << setw(2) << transition.first.first << ", " << -1 << ") -> ";
        }
        else
        {
            cout << "(" << setw(2) << transition.first.first << ", " << setw(2) << transition.first.second << ") -> ";
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

// 构造基础 NFA
NFA createBasicNFA(char symbol) {
    set<int> states = { 0, 1 };
    set<char> alphabet = { symbol };
    map<pair<int, char>, set<int>> transitions = { {{0, symbol}, {1}} };
    return NFA(states, alphabet, transitions, 0, { 1 });
}

// 连接两个 NFA
NFA concatNFA(const NFA& nfa1, const NFA& nfa2) {
    // 合并状态集合并调整 nfa2 的状态编号
    set<int> combinedStates = nfa1.states;
    int offset = *nfa1.states.rbegin() + 1;
    for (int state : nfa2.states) {
        combinedStates.insert(state + offset);
    }

    // 合并字母表
    set<char> newAlphabet = nfa1.alphabet;
    newAlphabet.insert(nfa2.alphabet.begin(), nfa2.alphabet.end());

    // 合并转移函数并调整 nfa2 的状态编号
    map<pair<int, char>, set<int>> newTransitions = nfa1.transitions;
    for (const auto& transition : nfa2.transitions) {
        pair<int, char> newKey = { transition.first.first + offset, transition.first.second };
        set<int> adjustedTargets;
        for (int target : transition.second) {
            adjustedTargets.insert(target + offset);
        }
        newTransitions[newKey] = adjustedTargets;
    }

    // 添加 ε 转移：nfa1 的终止状态连接到 nfa2 的起始状态
    for (int finalState : nfa1.finalStates) {
        newTransitions[{finalState, '-'}].insert(nfa2.startState + offset);
    }

    // 调整 nfa2 的终止状态为新的终止状态集合
    set<int> newFinalStates;
    for (int finalState : nfa2.finalStates) {
        newFinalStates.insert(finalState + offset);
    }

    // 返回连接后的 NFA
    return NFA(combinedStates, newAlphabet, newTransitions, nfa1.startState, newFinalStates);
}

// 合并两个 NFA（Union）
NFA unionNFA(const NFA& nfa1, const NFA& nfa2) {
    // 初始化合并后的 NFA
    NFA unionNFA;

    // 新的初始状态
    int newStartState = -1;
    unionNFA.states.insert(newStartState);
    unionNFA.startState = newStartState;

    // 合并状态集合
    for (int state : nfa1.states) {
        unionNFA.states.insert(state);
    }
    for (int state : nfa2.states) {
        unionNFA.states.insert(state);
    }

    // 合并字母表
    unionNFA.alphabet.insert(nfa1.alphabet.begin(), nfa1.alphabet.end());
    unionNFA.alphabet.insert(nfa2.alphabet.begin(), nfa2.alphabet.end());

    // 合并状态转换函数
    for (const auto& transition : nfa1.transitions) {
        int fromState = transition.first.first;
        char inputChar = transition.first.second;

        for (int toState : transition.second) {
            unionNFA.transitions[{fromState, inputChar}].insert(toState);
        }
    }
    for (const auto& transition : nfa2.transitions) {
        int fromState = transition.first.first;
        char inputChar = transition.first.second;

        for (int toState : transition.second) {
            unionNFA.transitions[{fromState, inputChar}].insert(toState);
        }
    }

    // 合并终止状态集
    unionNFA.finalStates.insert(nfa1.finalStates.begin(), nfa1.finalStates.end());
    unionNFA.finalStates.insert(nfa2.finalStates.begin(), nfa2.finalStates.end());

    // 添加 ε 转移从新的初始状态到两个 NFA 的起始状态
    unionNFA.transitions[{newStartState, '-'}].insert(nfa1.startState);
    unionNFA.transitions[{newStartState, '-'}].insert(nfa2.startState);

    return unionNFA;
}

// 闭包操作
NFA RE_closure(const NFA& nfa) {
    // 添加新的起始状态和终止状态
    set<int> newStates = nfa.states;
    int newStartState = *nfa.states.rbegin() + 1;
    int newFinalState = newStartState + 1;
    newStates.insert(newStartState);
    newStates.insert(newFinalState);

    // 拷贝原来的转移函数
    map<pair<int, char>, set<int>> newTransitions = nfa.transitions;

    // 添加 ε 转移：从新起始状态到原起始状态和新终止状态
    newTransitions[{newStartState, '-'}].insert(nfa.startState);
    newTransitions[{newStartState, '-'}].insert(newFinalState);

    // 添加 ε 转移：从原终止状态到新终止状态和原起始状态
    for (int finalState : nfa.finalStates) {
        newTransitions[{finalState, '-'}].insert(newFinalState);
        newTransitions[{finalState, '-'}].insert(nfa.startState);
    }

    // 返回应用 Kleene 星操作后的 NFA
    return NFA(newStates, nfa.alphabet, newTransitions, newStartState, { newFinalState });
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

