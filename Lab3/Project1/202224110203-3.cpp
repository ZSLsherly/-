//#include <iostream>
//#include <vector>
//#include <stack>
//#include <string>
//#include <cctype>
//#include <set>
//#include <map>
//#include <sstream>
//#include <fstream>
//#include <iomanip>
//#include <unordered_map>
//#include <queue>
//
//using namespace std;
//
//set<int> epsilonClosure(const set<int>& states, const map<pair<int, char>, set<int>>& transitions);
//set<int> move(const set<int>& states, char symbol, const map<pair<int, char>, set<int>>& transitions);
//
//struct DFA {
//    set<int> states;		// 状态集
//    set<char> alphabet;		// 输入字母表
//    map<pair<int, char>, int> transitions;		// 状态转换函数	
//    int startState;			// 开始状态
//    set<int> finalStates;	// 终止状态集
//    DFA(set<int> states, set<char> alphabet, map<pair<int, char>, int> transitions,
//        int startState, set<int> finalStates) :states(states), alphabet(alphabet), transitions(transitions),
//        startState(startState), finalStates(finalStates) {};
//    DFA() {};
//};
//
//struct NFA {
//    set<int> states;	// 有穷状态集
//    set<char> alphabet;	// 输入字母表
//    map<pair<int, char>, set<int>> transitions;	// 状态转换函数
//    int startState;		// 开始状态
//    set<int> finalStates;	// 终止状态集
//    NFA(set<int> states, set<char> alphabet, map<pair<int, char>, set<int>> transitions,
//        int startState, set<int> finalStates) :states(states), alphabet(alphabet), transitions(transitions),
//        startState(startState), finalStates(finalStates) {};
//    NFA() {};
//};
//
//NFA turnToNFA(const string& expression);
//string turnToPostfix(const string& input);
//void printNFA(NFA& nfa);
//NFA readNFA(string txt_path);
//DFA turnToDFA(NFA& nfa);
//void printDFA(DFA& dfa);
//DFA mininizedDFA(DFA& dfa);
//NFA createBasicNFA(char symbol);
//NFA concatNFA(const NFA& nfa1, const NFA& nfa2);
//NFA unionNFA(const NFA& nfa1, const NFA& nfa2);
//NFA RE_closure(const NFA& nfa);
//
//
//
//void printDFA(DFA& dfa)
//{
//    cout << "状态集:";
//    for (const int state : dfa.states) {
//        cout << state << " ";
//    }
//    cout << endl;
//
//    cout << "输入字母表:";
//    for (const char alpha : dfa.alphabet) {
//        cout << alpha << " ";
//    }
//    cout << endl;
//
//    cout << "状态转换函数:" << endl;
//    for (const auto& transition : dfa.transitions) {
//        cout << "(" << transition.first.first << ", " << transition.first.second << ") -> " << transition.second << endl;
//    }
//
//    cout << "初始状态:" << dfa.startState << endl;
//
//    cout << "终止状态集:";
//    for (const int state : dfa.finalStates) {
//        cout << state << " ";
//    }
//    cout << endl;
//}
//
//
//DFA turnToDFA(NFA& nfa)
//{
//    // 检查NFA是否已确定化
//    bool oneNFA = true;
//    for (const auto& transition : nfa.transitions) {
//        // 如果存在环或一个状态在某输入下有多个目标状态，则不是DFA
//        if (transition.first.second == '-' || transition.second.size() > 1) {
//            oneNFA = false;
//            break;
//        }
//    }
//
//    if (oneNFA) {
//        cout << endl << "NFA已确定化，直接转换为DFA" << endl;
//        set<int> dfaStates = nfa.states;
//        set<char> alphabet = nfa.alphabet;
//        map<pair<int, char>, int> transitions;
//
//        // 转换NFA的每个转移到DFA
//        for (const auto& transition : nfa.transitions) {
//            int fromState = transition.first.first;
//            char symbol = transition.first.second;
//            int toState = *transition.second.begin(); // 每个转移只有一个目标状态
//            transitions[{fromState, symbol}] = toState;
//        }
//
//        int startState = nfa.startState;
//        set<int> finalStates = nfa.finalStates;
//
//        return DFA(dfaStates, alphabet, transitions, startState, finalStates);
//    }
//
//    // 子集构造法进行NFA到DFA的转换
//    set<set<int>> dfaStates; // DFA的状态集（由NFA状态的子集组成）
//    set<char> alphabet = nfa.alphabet;
//    map<pair<set<int>, char>, set<int>> dfaTransitions; // DFA的转移函数
//
//    // 计算初始状态的ε闭包
//    set<int> startState = epsilonClosure({ nfa.startState }, nfa.transitions);
//    dfaStates.insert(startState);
//
//    queue<set<int>> stateQueue;
//    stateQueue.push(startState);
//
//    set<set<int>> finalStates; // DFA的终止状态集
//
//    // 逐步处理每个状态
//    while (!stateQueue.empty()) {
//        set<int> currentState = stateQueue.front();
//        stateQueue.pop();
//
//        for (char symbol : alphabet) {
//            // 计算当前状态集合在输入 symbol 下的目标状态集合
//            set<int> nextState = move(currentState, symbol, nfa.transitions);
//
//            if (!nextState.empty()) {
//                // 对目标状态集合计算ε闭包
//                nextState = epsilonClosure(nextState, nfa.transitions);
//
//                // 如果目标状态集合是新状态，加入DFA的状态集中
//                if (!dfaStates.count(nextState)) {
//                    dfaStates.insert(nextState);
//                    stateQueue.push(nextState);
//                }
//
//                // 更新DFA的转移函数
//                dfaTransitions[{currentState, symbol}] = nextState;
//            }
//        }
//
//        // 判断当前状态集合是否为终止状态
//        for (int state : currentState) {
//            if (nfa.finalStates.count(state)) {
//                finalStates.insert(currentState);
//                break;
//            }
//        }
//    }
//
//    // 状态集合映射为整数
//    map<set<int>, int> stateMapping;
//    int stateCount = 0;
//    set<int> smalledStates;
//
//    for (const auto& stateSet : dfaStates) {
//        stateMapping[stateSet] = stateCount++;
//        smalledStates.insert(stateMapping[stateSet]);
//    }
//
//    // 新的转移函数
//    map<pair<int, char>, int> smalledTransitions;
//    for (const auto& [key, value] : dfaTransitions) {
//        int fromState = stateMapping[key.first];
//        int toState = stateMapping[value];
//        smalledTransitions[{fromState, key.second}] = toState;
//    }
//
//    // 扁平化终止状态集合
//    set<int> smalledFinalStates;
//    for (const auto& stateSet : finalStates) {
//        smalledFinalStates.insert(stateMapping[stateSet]);
//    }
//
//    // 扁平化初始状态
//    int smalledStartState = stateMapping[startState];
//
//    // 返回确定化后的DFA
//    return DFA(smalledStates, alphabet, smalledTransitions, smalledStartState, smalledFinalStates);
//}
//
//set<int> epsilonClosure(const set<int>& states, const map<pair<int, char>, set<int>>& transitions) {
//    set<int> closure = states; // 初始化闭包为输入的状态集合
//    stack<int> stack;          // 用于深度优先搜索的栈
//
//    // 将所有初始状态压入栈
//    for (int state : states) {
//        stack.push(state);
//    }
//
//    // 深度优先搜索，遍历所有通过 ε 转移可以到达的状态
//    while (!stack.empty()) {
//        int currentState = stack.top();
//        stack.pop();
//
//        // 查找当前状态的 ε 转移
//        auto it = transitions.find({ currentState, '-' }); // -1 表示 ε 转移
//        if (it != transitions.end()) {
//            for (int nextState : it->second) {
//                // 如果闭包中尚未包含该状态，则加入闭包并压入栈
//                if (closure.find(nextState) == closure.end()) {
//                    closure.insert(nextState);
//                    stack.push(nextState);
//                }
//            }
//        }
//    }
//
//    return closure;
//}
//
//
//set<int> move(const set<int>& states, char symbol, const map<pair<int, char>, set<int>>& transitions) {
//    set<int> result; // 存储目标状态集合
//
//    // 遍历状态集合
//    for (int state : states) {
//        // 查找当前状态在指定符号下的转移
//        auto it = transitions.find({ state, symbol });
//        if (it != transitions.end()) {
//            // 将所有目标状态加入结果集合
//            result.insert(it->second.begin(), it->second.end());
//        }
//    }
//
//    return result;
//}
//
//DFA mininizedDFA(DFA& dfa) {
//    // 1. 初始化分组：终止状态和非终止状态
//    set<int> finalStates = dfa.finalStates;
//    set<int> nonFinalStates;
//    bool flag = true;
//    for (int state : dfa.states) {
//        if (!finalStates.count(state)) {
//            nonFinalStates.insert(state);
//        }
//    }
//
//    // 初始划分 P：{终止状态} 和 {非终止状态}
//    set<set<int>> partitions;
//    partitions.insert(finalStates);
//    partitions.insert(nonFinalStates);
//
//    // 2. 开始划分过程
//    while (flag) {
//        flag = false;
//        // 按照当前符号对分区进行划分
//        set<set<int>> newPartitions;
//        for (const auto& partition : partitions) {
//            map<set<pair<set<int>, char>>, set<int>> veri_mp;   // 存储同一分类的set   
//            for (auto state : partition) {
//                set<pair<set<int>, char>> veri_set; // 存储转化状态
//                for (char symbol : dfa.alphabet) {
//                    if (dfa.transitions.count({ state,symbol })) {
//                        int temp = dfa.transitions[{ state, symbol }];
//                        for (auto partition_temp : partitions) {
//                            if (partition_temp.count(temp)) {
//                                veri_set.insert({ partition_temp, symbol });
//                                break;
//                            }
//                        }
//                    }
//                }
//                veri_mp[veri_set].insert(state);
//            }
//            for (auto temp : veri_mp) {
//                newPartitions.insert(temp.second);
//            }
//        }
//        // 更新分区
//        if (newPartitions.size() > partitions.size()) {
//            partitions = newPartitions;
//            flag = true;
//        }
//    }
//
//    // 3. 构造最小化 DFA
//    map<set<int>, int> stateMapping; // 用于映射每个状态分组到新状态
//    int newStateCounter = 0;
//    for (const auto& partition : partitions) {
//        stateMapping[partition] = newStateCounter++;
//    }
//
//    set<int> newStates;
//    set<int> newFinalStates;
//    map<pair<int, char>, int> newTransitions;
//    int newStartState = -1;
//
//    // 构造新的状态、转移函数、初始状态和终止状态
//    for (const auto& partition : partitions) {
//        int newState = stateMapping[partition];
//        newStates.insert(newState);
//
//        // 如果原分组包含初始状态，设置新的初始状态
//        if (partition.count(dfa.startState)) {
//            newStartState = newState;
//        }
//
//        // 如果分组包含终止状态，加入新的终止状态集
//        for (int state : partition) {
//            if (finalStates.count(state)) {
//                newFinalStates.insert(newState);
//                break;
//            }
//        }
//
//        // 构造转移函数
//        for (int state : partition) {
//            for (char symbol : dfa.alphabet) {
//                auto it = dfa.transitions.find({ state, symbol });
//                if (it != dfa.transitions.end()) {
//                    int targetState = it->second;
//
//                    // 找到目标状态所属的分组
//                    for (const auto& targetPartition : partitions) {
//                        if (targetPartition.count(targetState)) {
//                            newTransitions[{newState, symbol}] = stateMapping[targetPartition];
//                            break;
//                        }
//                    }
//                }
//            }
//        }
//    }
//
//    // 返回最小化 DFA
//    return DFA(newStates, dfa.alphabet, newTransitions, newStartState, newFinalStates);
//}
//
//
//map<char, int> operater_num;
//
//// 初始化操作符优先级
//void init_operater_num() {
//    operater_num['*'] = 3;
//    operater_num['.'] = 2;
//    operater_num['|'] = 1;
//}
//
//// 中缀表达式转换为后缀表达式（逆波兰表达式）
//string turnToPostfix(const string& input) {
//    init_operater_num();
//    string temp = "";
//    for (size_t i = 0; i < input.length(); i++) {
//        temp += input[i];
//
//        if (i + 1 < input.length()) {
//            char current = input[i];
//            char next = input[i + 1];
//
//            // 添加显式连接符的条件
//            if ((isalnum(current) || current == ')' || current == '*') &&
//                (isalnum(next) || next == '(')) {
//                temp += '.';
//            }
//        }
//    }
//    stack<char> operatorStack;
//    string postfix = "";
//
//    for (char ch : temp) {
//        if (isalnum(ch)) {
//            postfix += ch;  // 直接输出操作数
//        }
//        else if (ch == '(') {
//            operatorStack.push(ch);  // 左括号入栈
//        }
//        else if (ch == ')') {
//            // 弹出直到遇到左括号
//            while (!operatorStack.empty() && operatorStack.top() != '(') {
//                postfix += operatorStack.top();
//                operatorStack.pop();
//            }
//            operatorStack.pop();  // 弹出左括号
//        }
//        else if (ch == '|' || ch == '*' || ch == '.') {
//            // 根据优先级处理操作符
//            while (!operatorStack.empty() && operater_num[operatorStack.top()] >= operater_num[ch]) {
//                postfix += operatorStack.top();
//                operatorStack.pop();
//            }
//            operatorStack.push(ch);
//        }
//    }
//
//    // 处理栈中剩余的操作符
//    while (!operatorStack.empty()) {
//        postfix += operatorStack.top();
//        operatorStack.pop();
//    }
//
//    return postfix;
//}
//
//// 从后缀表达式生成 NFA
//NFA turnToNFA(const string& postfix) {
//    stack<NFA> nfaStack;
//
//    for (char token : postfix) {
//        if (isalnum(token)) {
//            nfaStack.push(createBasicNFA(token));  // 基本的 NFA
//        }
//        else if (token == '*') {
//            // 克林闭包
//            NFA nfa = nfaStack.top(); nfaStack.pop();
//            nfaStack.push(RE_closure(nfa));
//        }
//        else if (token == '.') {
//            // 连接操作
//            NFA nfa2 = nfaStack.top(); nfaStack.pop();
//            NFA nfa1 = nfaStack.top(); nfaStack.pop();
//            nfaStack.push(concatNFA(nfa1, nfa2));
//        }
//        else if (token == '|') {
//            // 并集操作
//            NFA nfa2 = nfaStack.top(); nfaStack.pop();
//            NFA nfa1 = nfaStack.top(); nfaStack.pop();
//            nfaStack.push(unionNFA(nfa1, nfa2));
//        }
//    }
//    return nfaStack.top();  // 最终的 NFA
//}
//
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
//            cout << "(" << setw(2) << transition.first.first << ", " << -1 << ") -> ";
//        }
//        else
//        {
//            cout << "(" << setw(2) << transition.first.first << ", " << setw(2) << transition.first.second << ") -> ";
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
//// 构造基础 NFA
//NFA createBasicNFA(char symbol) {
//    set<int> states = { 0, 1 };
//    set<char> alphabet = { symbol };
//    map<pair<int, char>, set<int>> transitions = { {{0, symbol}, {1}} };
//    return NFA(states, alphabet, transitions, 0, { 1 });
//}
//
//// 连接两个 NFA
//NFA concatNFA(const NFA& nfa1, const NFA& nfa2) {
//    // 合并状态集合并调整 nfa2 的状态编号
//    set<int> combinedStates = nfa1.states;
//    int offset = *nfa1.states.rbegin() + 1;
//    for (int state : nfa2.states) {
//        combinedStates.insert(state + offset);
//    }
//
//    // 合并字母表
//    set<char> newAlphabet = nfa1.alphabet;
//    newAlphabet.insert(nfa2.alphabet.begin(), nfa2.alphabet.end());
//
//    // 合并转移函数并调整 nfa2 的状态编号
//    map<pair<int, char>, set<int>> newTransitions = nfa1.transitions;
//    for (const auto& transition : nfa2.transitions) {
//        pair<int, char> newKey = { transition.first.first + offset, transition.first.second };
//        set<int> adjustedTargets;
//        for (int target : transition.second) {
//            adjustedTargets.insert(target + offset);
//        }
//        newTransitions[newKey] = adjustedTargets;
//    }
//
//    // 添加 ε 转移：nfa1 的终止状态连接到 nfa2 的起始状态
//    for (int finalState : nfa1.finalStates) {
//        newTransitions[{finalState, '-'}].insert(nfa2.startState + offset);
//    }
//
//    // 调整 nfa2 的终止状态为新的终止状态集合
//    set<int> newFinalStates;
//    for (int finalState : nfa2.finalStates) {
//        newFinalStates.insert(finalState + offset);
//    }
//
//    // 返回连接后的 NFA
//    return NFA(combinedStates, newAlphabet, newTransitions, nfa1.startState, newFinalStates);
//}
//
//// 合并两个 NFA（Union）
//NFA unionNFA(const NFA& nfa1, const NFA& nfa2) {
//    // 初始化合并后的 NFA
//    NFA unionNFA;
//
//    // 新的初始状态
//    int newStartState = -1;
//    unionNFA.states.insert(newStartState);
//    unionNFA.startState = newStartState;
//
//    // 合并状态集合
//    for (int state : nfa1.states) {
//        unionNFA.states.insert(state);
//    }
//    for (int state : nfa2.states) {
//        unionNFA.states.insert(state);
//    }
//
//    // 合并字母表
//    unionNFA.alphabet.insert(nfa1.alphabet.begin(), nfa1.alphabet.end());
//    unionNFA.alphabet.insert(nfa2.alphabet.begin(), nfa2.alphabet.end());
//
//    // 合并状态转换函数
//    for (const auto& transition : nfa1.transitions) {
//        int fromState = transition.first.first;
//        char inputChar = transition.first.second;
//
//        for (int toState : transition.second) {
//            unionNFA.transitions[{fromState, inputChar}].insert(toState);
//        }
//    }
//    for (const auto& transition : nfa2.transitions) {
//        int fromState = transition.first.first;
//        char inputChar = transition.first.second;
//
//        for (int toState : transition.second) {
//            unionNFA.transitions[{fromState, inputChar}].insert(toState);
//        }
//    }
//
//    // 合并终止状态集
//    unionNFA.finalStates.insert(nfa1.finalStates.begin(), nfa1.finalStates.end());
//    unionNFA.finalStates.insert(nfa2.finalStates.begin(), nfa2.finalStates.end());
//
//    // 添加 ε 转移从新的初始状态到两个 NFA 的起始状态
//    unionNFA.transitions[{newStartState, '-'}].insert(nfa1.startState);
//    unionNFA.transitions[{newStartState, '-'}].insert(nfa2.startState);
//
//    return unionNFA;
//}
//
//// 闭包操作
//NFA RE_closure(const NFA& nfa) {
//    // 添加新的起始状态和终止状态
//    set<int> newStates = nfa.states;
//    int newStartState = *nfa.states.rbegin() + 1;
//    int newFinalState = newStartState + 1;
//    newStates.insert(newStartState);
//    newStates.insert(newFinalState);
//
//    // 拷贝原来的转移函数
//    map<pair<int, char>, set<int>> newTransitions = nfa.transitions;
//
//    // 添加 ε 转移：从新起始状态到原起始状态和新终止状态
//    newTransitions[{newStartState, '-'}].insert(nfa.startState);
//    newTransitions[{newStartState, '-'}].insert(newFinalState);
//
//    // 添加 ε 转移：从原终止状态到新终止状态和原起始状态
//    for (int finalState : nfa.finalStates) {
//        newTransitions[{finalState, '-'}].insert(newFinalState);
//        newTransitions[{finalState, '-'}].insert(nfa.startState);
//    }
//
//    // 返回应用 Kleene 星操作后的 NFA
//    return NFA(newStates, nfa.alphabet, newTransitions, newStartState, { newFinalState });
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
//
//int main(int argc, char* argv[]) {
//    if (argc < 2)
//    {
//        cerr << "未输入RE" << endl;
//        return 1;
//    }
//
//    string RE = argv[1];
//    cout << "输入的正规式:" << endl;
//    cout << RE << endl;
//
//    string postfix = turnToPostfix(RE);
//    cout << "转化后的后缀表达式" << endl;
//    cout << postfix << endl;
//
//    NFA nfa = turnToNFA(postfix);
//    cout << "转换后的NFA:" << endl;
//    printNFA(nfa);
//
//    DFA dfa = turnToDFA(nfa);
//    cout << endl << "NFA转成DFA" << endl;
//    printDFA(dfa);
//
//    dfa = mininizedDFA(dfa);
//    cout << endl << "最小化后的DFA" << endl;
//    printDFA(dfa);
//
//    return 0;
//}
//
