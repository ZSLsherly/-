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
//    set<int> states;		// ״̬��
//    set<char> alphabet;		// ������ĸ��
//    map<pair<int, char>, int> transitions;		// ״̬ת������	
//    int startState;			// ��ʼ״̬
//    set<int> finalStates;	// ��ֹ״̬��
//    DFA(set<int> states, set<char> alphabet, map<pair<int, char>, int> transitions,
//        int startState, set<int> finalStates) :states(states), alphabet(alphabet), transitions(transitions),
//        startState(startState), finalStates(finalStates) {};
//    DFA() {};
//};
//
//struct NFA {
//    set<int> states;	// ����״̬��
//    set<char> alphabet;	// ������ĸ��
//    map<pair<int, char>, set<int>> transitions;	// ״̬ת������
//    int startState;		// ��ʼ״̬
//    set<int> finalStates;	// ��ֹ״̬��
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
//    cout << "״̬��:";
//    for (const int state : dfa.states) {
//        cout << state << " ";
//    }
//    cout << endl;
//
//    cout << "������ĸ��:";
//    for (const char alpha : dfa.alphabet) {
//        cout << alpha << " ";
//    }
//    cout << endl;
//
//    cout << "״̬ת������:" << endl;
//    for (const auto& transition : dfa.transitions) {
//        cout << "(" << transition.first.first << ", " << transition.first.second << ") -> " << transition.second << endl;
//    }
//
//    cout << "��ʼ״̬:" << dfa.startState << endl;
//
//    cout << "��ֹ״̬��:";
//    for (const int state : dfa.finalStates) {
//        cout << state << " ";
//    }
//    cout << endl;
//}
//
//
//DFA turnToDFA(NFA& nfa)
//{
//    // ���NFA�Ƿ���ȷ����
//    bool oneNFA = true;
//    for (const auto& transition : nfa.transitions) {
//        // ������ڻ���һ��״̬��ĳ�������ж��Ŀ��״̬������DFA
//        if (transition.first.second == '-' || transition.second.size() > 1) {
//            oneNFA = false;
//            break;
//        }
//    }
//
//    if (oneNFA) {
//        cout << endl << "NFA��ȷ������ֱ��ת��ΪDFA" << endl;
//        set<int> dfaStates = nfa.states;
//        set<char> alphabet = nfa.alphabet;
//        map<pair<int, char>, int> transitions;
//
//        // ת��NFA��ÿ��ת�Ƶ�DFA
//        for (const auto& transition : nfa.transitions) {
//            int fromState = transition.first.first;
//            char symbol = transition.first.second;
//            int toState = *transition.second.begin(); // ÿ��ת��ֻ��һ��Ŀ��״̬
//            transitions[{fromState, symbol}] = toState;
//        }
//
//        int startState = nfa.startState;
//        set<int> finalStates = nfa.finalStates;
//
//        return DFA(dfaStates, alphabet, transitions, startState, finalStates);
//    }
//
//    // �Ӽ����취����NFA��DFA��ת��
//    set<set<int>> dfaStates; // DFA��״̬������NFA״̬���Ӽ���ɣ�
//    set<char> alphabet = nfa.alphabet;
//    map<pair<set<int>, char>, set<int>> dfaTransitions; // DFA��ת�ƺ���
//
//    // �����ʼ״̬�Ħűհ�
//    set<int> startState = epsilonClosure({ nfa.startState }, nfa.transitions);
//    dfaStates.insert(startState);
//
//    queue<set<int>> stateQueue;
//    stateQueue.push(startState);
//
//    set<set<int>> finalStates; // DFA����ֹ״̬��
//
//    // �𲽴���ÿ��״̬
//    while (!stateQueue.empty()) {
//        set<int> currentState = stateQueue.front();
//        stateQueue.pop();
//
//        for (char symbol : alphabet) {
//            // ���㵱ǰ״̬���������� symbol �µ�Ŀ��״̬����
//            set<int> nextState = move(currentState, symbol, nfa.transitions);
//
//            if (!nextState.empty()) {
//                // ��Ŀ��״̬���ϼ���űհ�
//                nextState = epsilonClosure(nextState, nfa.transitions);
//
//                // ���Ŀ��״̬��������״̬������DFA��״̬����
//                if (!dfaStates.count(nextState)) {
//                    dfaStates.insert(nextState);
//                    stateQueue.push(nextState);
//                }
//
//                // ����DFA��ת�ƺ���
//                dfaTransitions[{currentState, symbol}] = nextState;
//            }
//        }
//
//        // �жϵ�ǰ״̬�����Ƿ�Ϊ��ֹ״̬
//        for (int state : currentState) {
//            if (nfa.finalStates.count(state)) {
//                finalStates.insert(currentState);
//                break;
//            }
//        }
//    }
//
//    // ״̬����ӳ��Ϊ����
//    map<set<int>, int> stateMapping;
//    int stateCount = 0;
//    set<int> smalledStates;
//
//    for (const auto& stateSet : dfaStates) {
//        stateMapping[stateSet] = stateCount++;
//        smalledStates.insert(stateMapping[stateSet]);
//    }
//
//    // �µ�ת�ƺ���
//    map<pair<int, char>, int> smalledTransitions;
//    for (const auto& [key, value] : dfaTransitions) {
//        int fromState = stateMapping[key.first];
//        int toState = stateMapping[value];
//        smalledTransitions[{fromState, key.second}] = toState;
//    }
//
//    // ��ƽ����ֹ״̬����
//    set<int> smalledFinalStates;
//    for (const auto& stateSet : finalStates) {
//        smalledFinalStates.insert(stateMapping[stateSet]);
//    }
//
//    // ��ƽ����ʼ״̬
//    int smalledStartState = stateMapping[startState];
//
//    // ����ȷ�������DFA
//    return DFA(smalledStates, alphabet, smalledTransitions, smalledStartState, smalledFinalStates);
//}
//
//set<int> epsilonClosure(const set<int>& states, const map<pair<int, char>, set<int>>& transitions) {
//    set<int> closure = states; // ��ʼ���հ�Ϊ�����״̬����
//    stack<int> stack;          // �����������������ջ
//
//    // �����г�ʼ״̬ѹ��ջ
//    for (int state : states) {
//        stack.push(state);
//    }
//
//    // ���������������������ͨ�� �� ת�ƿ��Ե����״̬
//    while (!stack.empty()) {
//        int currentState = stack.top();
//        stack.pop();
//
//        // ���ҵ�ǰ״̬�� �� ת��
//        auto it = transitions.find({ currentState, '-' }); // -1 ��ʾ �� ת��
//        if (it != transitions.end()) {
//            for (int nextState : it->second) {
//                // ����հ�����δ������״̬�������հ���ѹ��ջ
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
//    set<int> result; // �洢Ŀ��״̬����
//
//    // ����״̬����
//    for (int state : states) {
//        // ���ҵ�ǰ״̬��ָ�������µ�ת��
//        auto it = transitions.find({ state, symbol });
//        if (it != transitions.end()) {
//            // ������Ŀ��״̬����������
//            result.insert(it->second.begin(), it->second.end());
//        }
//    }
//
//    return result;
//}
//
//DFA mininizedDFA(DFA& dfa) {
//    // 1. ��ʼ�����飺��ֹ״̬�ͷ���ֹ״̬
//    set<int> finalStates = dfa.finalStates;
//    set<int> nonFinalStates;
//    bool flag = true;
//    for (int state : dfa.states) {
//        if (!finalStates.count(state)) {
//            nonFinalStates.insert(state);
//        }
//    }
//
//    // ��ʼ���� P��{��ֹ״̬} �� {����ֹ״̬}
//    set<set<int>> partitions;
//    partitions.insert(finalStates);
//    partitions.insert(nonFinalStates);
//
//    // 2. ��ʼ���ֹ���
//    while (flag) {
//        flag = false;
//        // ���յ�ǰ���ŶԷ������л���
//        set<set<int>> newPartitions;
//        for (const auto& partition : partitions) {
//            map<set<pair<set<int>, char>>, set<int>> veri_mp;   // �洢ͬһ�����set   
//            for (auto state : partition) {
//                set<pair<set<int>, char>> veri_set; // �洢ת��״̬
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
//        // ���·���
//        if (newPartitions.size() > partitions.size()) {
//            partitions = newPartitions;
//            flag = true;
//        }
//    }
//
//    // 3. ������С�� DFA
//    map<set<int>, int> stateMapping; // ����ӳ��ÿ��״̬���鵽��״̬
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
//    // �����µ�״̬��ת�ƺ�������ʼ״̬����ֹ״̬
//    for (const auto& partition : partitions) {
//        int newState = stateMapping[partition];
//        newStates.insert(newState);
//
//        // ���ԭ���������ʼ״̬�������µĳ�ʼ״̬
//        if (partition.count(dfa.startState)) {
//            newStartState = newState;
//        }
//
//        // ������������ֹ״̬�������µ���ֹ״̬��
//        for (int state : partition) {
//            if (finalStates.count(state)) {
//                newFinalStates.insert(newState);
//                break;
//            }
//        }
//
//        // ����ת�ƺ���
//        for (int state : partition) {
//            for (char symbol : dfa.alphabet) {
//                auto it = dfa.transitions.find({ state, symbol });
//                if (it != dfa.transitions.end()) {
//                    int targetState = it->second;
//
//                    // �ҵ�Ŀ��״̬�����ķ���
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
//    // ������С�� DFA
//    return DFA(newStates, dfa.alphabet, newTransitions, newStartState, newFinalStates);
//}
//
//
//map<char, int> operater_num;
//
//// ��ʼ�����������ȼ�
//void init_operater_num() {
//    operater_num['*'] = 3;
//    operater_num['.'] = 2;
//    operater_num['|'] = 1;
//}
//
//// ��׺���ʽת��Ϊ��׺���ʽ���沨�����ʽ��
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
//            // �����ʽ���ӷ�������
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
//            postfix += ch;  // ֱ�����������
//        }
//        else if (ch == '(') {
//            operatorStack.push(ch);  // ��������ջ
//        }
//        else if (ch == ')') {
//            // ����ֱ������������
//            while (!operatorStack.empty() && operatorStack.top() != '(') {
//                postfix += operatorStack.top();
//                operatorStack.pop();
//            }
//            operatorStack.pop();  // ����������
//        }
//        else if (ch == '|' || ch == '*' || ch == '.') {
//            // �������ȼ����������
//            while (!operatorStack.empty() && operater_num[operatorStack.top()] >= operater_num[ch]) {
//                postfix += operatorStack.top();
//                operatorStack.pop();
//            }
//            operatorStack.push(ch);
//        }
//    }
//
//    // ����ջ��ʣ��Ĳ�����
//    while (!operatorStack.empty()) {
//        postfix += operatorStack.top();
//        operatorStack.pop();
//    }
//
//    return postfix;
//}
//
//// �Ӻ�׺���ʽ���� NFA
//NFA turnToNFA(const string& postfix) {
//    stack<NFA> nfaStack;
//
//    for (char token : postfix) {
//        if (isalnum(token)) {
//            nfaStack.push(createBasicNFA(token));  // ������ NFA
//        }
//        else if (token == '*') {
//            // ���ֱհ�
//            NFA nfa = nfaStack.top(); nfaStack.pop();
//            nfaStack.push(RE_closure(nfa));
//        }
//        else if (token == '.') {
//            // ���Ӳ���
//            NFA nfa2 = nfaStack.top(); nfaStack.pop();
//            NFA nfa1 = nfaStack.top(); nfaStack.pop();
//            nfaStack.push(concatNFA(nfa1, nfa2));
//        }
//        else if (token == '|') {
//            // ��������
//            NFA nfa2 = nfaStack.top(); nfaStack.pop();
//            NFA nfa1 = nfaStack.top(); nfaStack.pop();
//            nfaStack.push(unionNFA(nfa1, nfa2));
//        }
//    }
//    return nfaStack.top();  // ���յ� NFA
//}
//
//
//// ��ӡnfa
//void printNFA(NFA& nfa)
//{
//    cout << "״̬��:";
//    for (const int state : nfa.states)
//    {
//        cout << state << " ";
//    }
//    cout << endl << "������ĸ��:";
//    for (const char alpha : nfa.alphabet)
//    {
//        cout << alpha << " ";
//    }
//    cout << endl << "״̬ת������:" << endl;
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
//    cout << "��ʼ״̬:" << nfa.startState << endl;
//    cout << "��ֹ״̬��:";
//    for (const int state : nfa.finalStates)
//    {
//        cout << state << " ";
//    }
//    cout << endl;
//}
//
//// ������� NFA
//NFA createBasicNFA(char symbol) {
//    set<int> states = { 0, 1 };
//    set<char> alphabet = { symbol };
//    map<pair<int, char>, set<int>> transitions = { {{0, symbol}, {1}} };
//    return NFA(states, alphabet, transitions, 0, { 1 });
//}
//
//// �������� NFA
//NFA concatNFA(const NFA& nfa1, const NFA& nfa2) {
//    // �ϲ�״̬���ϲ����� nfa2 ��״̬���
//    set<int> combinedStates = nfa1.states;
//    int offset = *nfa1.states.rbegin() + 1;
//    for (int state : nfa2.states) {
//        combinedStates.insert(state + offset);
//    }
//
//    // �ϲ���ĸ��
//    set<char> newAlphabet = nfa1.alphabet;
//    newAlphabet.insert(nfa2.alphabet.begin(), nfa2.alphabet.end());
//
//    // �ϲ�ת�ƺ��������� nfa2 ��״̬���
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
//    // ��� �� ת�ƣ�nfa1 ����ֹ״̬���ӵ� nfa2 ����ʼ״̬
//    for (int finalState : nfa1.finalStates) {
//        newTransitions[{finalState, '-'}].insert(nfa2.startState + offset);
//    }
//
//    // ���� nfa2 ����ֹ״̬Ϊ�µ���ֹ״̬����
//    set<int> newFinalStates;
//    for (int finalState : nfa2.finalStates) {
//        newFinalStates.insert(finalState + offset);
//    }
//
//    // �������Ӻ�� NFA
//    return NFA(combinedStates, newAlphabet, newTransitions, nfa1.startState, newFinalStates);
//}
//
//// �ϲ����� NFA��Union��
//NFA unionNFA(const NFA& nfa1, const NFA& nfa2) {
//    // ��ʼ���ϲ���� NFA
//    NFA unionNFA;
//
//    // �µĳ�ʼ״̬
//    int newStartState = -1;
//    unionNFA.states.insert(newStartState);
//    unionNFA.startState = newStartState;
//
//    // �ϲ�״̬����
//    for (int state : nfa1.states) {
//        unionNFA.states.insert(state);
//    }
//    for (int state : nfa2.states) {
//        unionNFA.states.insert(state);
//    }
//
//    // �ϲ���ĸ��
//    unionNFA.alphabet.insert(nfa1.alphabet.begin(), nfa1.alphabet.end());
//    unionNFA.alphabet.insert(nfa2.alphabet.begin(), nfa2.alphabet.end());
//
//    // �ϲ�״̬ת������
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
//    // �ϲ���ֹ״̬��
//    unionNFA.finalStates.insert(nfa1.finalStates.begin(), nfa1.finalStates.end());
//    unionNFA.finalStates.insert(nfa2.finalStates.begin(), nfa2.finalStates.end());
//
//    // ��� �� ת�ƴ��µĳ�ʼ״̬������ NFA ����ʼ״̬
//    unionNFA.transitions[{newStartState, '-'}].insert(nfa1.startState);
//    unionNFA.transitions[{newStartState, '-'}].insert(nfa2.startState);
//
//    return unionNFA;
//}
//
//// �հ�����
//NFA RE_closure(const NFA& nfa) {
//    // ����µ���ʼ״̬����ֹ״̬
//    set<int> newStates = nfa.states;
//    int newStartState = *nfa.states.rbegin() + 1;
//    int newFinalState = newStartState + 1;
//    newStates.insert(newStartState);
//    newStates.insert(newFinalState);
//
//    // ����ԭ����ת�ƺ���
//    map<pair<int, char>, set<int>> newTransitions = nfa.transitions;
//
//    // ��� �� ת�ƣ�������ʼ״̬��ԭ��ʼ״̬������ֹ״̬
//    newTransitions[{newStartState, '-'}].insert(nfa.startState);
//    newTransitions[{newStartState, '-'}].insert(newFinalState);
//
//    // ��� �� ת�ƣ���ԭ��ֹ״̬������ֹ״̬��ԭ��ʼ״̬
//    for (int finalState : nfa.finalStates) {
//        newTransitions[{finalState, '-'}].insert(newFinalState);
//        newTransitions[{finalState, '-'}].insert(nfa.startState);
//    }
//
//    // ����Ӧ�� Kleene �ǲ������ NFA
//    return NFA(newStates, nfa.alphabet, newTransitions, newStartState, { newFinalState });
//}
//
//// ��ȡnfa
//NFA readNFA(string txt_path) {
//    ifstream file(txt_path); // ���ļ�
//    if (!file.is_open()) {
//        cerr << "�޷����ļ���" << endl;
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
//    // ��ȡ״̬��
//    if (getline(file, line)) {
//        stringstream ss(line);
//        while (ss >> num) {
//            states.insert(num);
//        }
//    }
//
//    // ��ȡ������ĸ��
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
//            if (line.empty()) break; // ������������ѭ��
//            stringstream ss(line);
//            int fromState, toState;
//            char inputChar;
//            set<int> toStates;
//
//            ss >> fromState >> inputChar;
//
//            // �����ַ�Ϊ `'-'` �����
//            if (inputChar == '-') {
//                ss >> inputChar;
//                inputChar = '-';
//            }
//
//            // ��ȡĿ��״̬����
//            while (ss >> toState) {
//                toStates.insert(toState);
//            }
//
//            // ��ת�ƹ�ϵ���� `transitions`
//            transitions[{fromState, inputChar}] = toStates;
//
//        }
//    }
//    // ��ȡ״̬ת������
//
//
//    // ��ȡ��ʼ״̬
//    if (getline(file, line)) {
//        stringstream ss(line);
//        ss >> startState;
//    }
//
//    // ��ȡ��ֹ״̬��
//    if (getline(file, line)) {
//        stringstream ss(line);
//        while (ss >> num) {
//            finalStates.insert(num);
//        }
//    }
//
//    file.close(); // �ر��ļ�
//
//    // ����NFA���󲢷���
//    NFA nfa(states, alphabet, transitions, startState, finalStates);
//    return nfa;
//}
//
//
//int main(int argc, char* argv[]) {
//    if (argc < 2)
//    {
//        cerr << "δ����RE" << endl;
//        return 1;
//    }
//
//    string RE = argv[1];
//    cout << "���������ʽ:" << endl;
//    cout << RE << endl;
//
//    string postfix = turnToPostfix(RE);
//    cout << "ת����ĺ�׺���ʽ" << endl;
//    cout << postfix << endl;
//
//    NFA nfa = turnToNFA(postfix);
//    cout << "ת�����NFA:" << endl;
//    printNFA(nfa);
//
//    DFA dfa = turnToDFA(nfa);
//    cout << endl << "NFAת��DFA" << endl;
//    printDFA(dfa);
//
//    dfa = mininizedDFA(dfa);
//    cout << endl << "��С�����DFA" << endl;
//    printDFA(dfa);
//
//    return 0;
//}
//
