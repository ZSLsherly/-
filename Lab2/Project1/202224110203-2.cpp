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
    set<int> states;		// ״̬��
    set<char> alphabet;		// ������ĸ��
    map<pair<int, char>, int> transitions;		// ״̬ת������	
    int startState;			// ��ʼ״̬
    set<int> finalStates;	// ��ֹ״̬��
    DFA(set<int> states, set<char> alphabet, map<pair<int, char>, int> transitions,
        int startState, set<int> finalStates) :states(states), alphabet(alphabet), transitions(transitions),
        startState(startState), finalStates(finalStates) {};
};

struct NFA {
    set<int> states;	// ����״̬��
    set<char> alphabet;	// ������ĸ��
    map<pair<int, char>, set<int>> transitions;	// ״̬ת������
    int startState;		// ��ʼ״̬
    set<int> finalStates;	// ��ֹ״̬��
    NFA(set<int> states, set<char> alphabet, map<pair<int, char>, set<int>> transitions,
        int startState, set<int> finalStates) :states(states), alphabet(alphabet), transitions(transitions),
        startState(startState), finalStates(finalStates) {};
    NFA() {};
};
// ��ӡnfa
void printNFA(NFA& nfa)
{
    cout << "״̬��:";
    for (const int state : nfa.states)
    {
        cout << state << " ";
    }
    cout << endl << "������ĸ��:";
    for (const char alpha : nfa.alphabet)
    {
        cout << alpha << " ";
    }
    cout << endl << "״̬ת������:" << endl;
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
    cout << "��ʼ״̬:" << nfa.startState << endl;
    cout << "��ֹ״̬��:";
    for (const int state : nfa.finalStates)
    {
        cout << state << " ";
    }
    cout << endl;
}

// ��ȡnfa
NFA readNFA(string txt_path) {
    ifstream file(txt_path); // ���ļ�
    if (!file.is_open()) {
        cerr << "�޷����ļ���" << endl;
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

    // ��ȡ״̬��
    if (getline(file, line)) {
        stringstream ss(line);
        while (ss >> num) {
            states.insert(num);
        }
    }

    // ��ȡ������ĸ��
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
            if (line.empty()) break; // ������������ѭ��
            stringstream ss(line);
            int fromState, toState;
            char inputChar;
            set<int> toStates;

            ss >> fromState >> inputChar;

            // �����ַ�Ϊ `'-'` �����
            if (inputChar == '-') {
                ss >> inputChar;
                inputChar = '-';
            }

            // ��ȡĿ��״̬����
            while (ss >> toState) {
                toStates.insert(toState);
            }

            // ��ת�ƹ�ϵ���� `transitions`
            transitions[{fromState, inputChar}] = toStates;

        }
    }
    // ��ȡ״̬ת������


    // ��ȡ��ʼ״̬
    if (getline(file, line)) {
        stringstream ss(line);
        ss >> startState;
    }

    // ��ȡ��ֹ״̬��
    if (getline(file, line)) {
        stringstream ss(line);
        while (ss >> num) {
            finalStates.insert(num);
        }
    }

    file.close(); // �ر��ļ�

    // ����NFA���󲢷���
    NFA nfa(states, alphabet, transitions, startState, finalStates);
    return nfa;
}

// �ϲ����nfa
NFA dealNFAS(vector<NFA>& nfas)
{
    if (nfas.empty())
    {
        cout << "û��NFA���Ժϲ���" << endl;
        exit(1);
    }
    // ��ʼ���ϲ����NFA
    NFA nfa;
    // �µĳ�ʼ״̬
    int newStartState = -1;
    nfa.states.insert(newStartState);
    nfa.startState = newStartState;
    for (const NFA& nfa_temp : nfas)
    {
        for (int state : nfa_temp.states)
        {
            nfa.states.insert(state);
        }

        // �ϲ���ĸ��
        nfa.alphabet.insert(nfa_temp.alphabet.begin(), nfa_temp.alphabet.end());

        // �ϲ�״̬ת������
        for (const auto& transition : nfa_temp.transitions)
        {
            int fromState = transition.first.first;
            char inputChar = transition.first.second;

            for (int toState : transition.second)
            {
                nfa.transitions[{fromState, inputChar}].insert(toState);
            }
        }

        // �ϲ���ֹ״̬��
        for (int finalState : nfa_temp.finalStates)
        {
            nfa.finalStates.insert(finalState);
        }

        // ��Ӧ�ת�Ƶ�ԭʼNFA����ʼ״̬
        nfa.transitions[{newStartState, '-'}].insert(nfa_temp.startState);

    }

    return nfa;
}


set<int> epsilonClosure(const set<int>& states, const map<pair<int, char>, set<int>>& transitions);
set<int> move(const set<int>& states, char symbol, const map<pair<int, char>, set<int>>& transitions);

void printDFA(DFA& dfa)
{
    cout << "״̬��:";
    for (const int state : dfa.states) {
        cout << state << " ";
    }
    cout << endl;

    cout << "������ĸ��:";
    for (const char alpha : dfa.alphabet) {
        cout << alpha << " ";
    }
    cout << endl;

    cout << "״̬ת������:" << endl;
    for (const auto& transition : dfa.transitions) {
        cout << "(" << transition.first.first << ", " << transition.first.second << ") -> " << transition.second << endl;
    }

    cout << "��ʼ״̬:" << dfa.startState << endl;

    cout << "��ֹ״̬��:";
    for (const int state : dfa.finalStates) {
        cout << state << " ";
    }
    cout << endl;
}


DFA transToDFA(NFA& nfa)
{
    // ���NFA�Ƿ���ȷ����
    bool oneNFA = true;
    for (const auto& transition : nfa.transitions) {
        // ������ڻ���һ��״̬��ĳ�������ж��Ŀ��״̬������DFA
        if (transition.first.second == '-' || transition.second.size() > 1) {
            oneNFA = false;
            break;
        }
    }

    if (oneNFA) {
        cout << endl << "NFA��ȷ������ֱ��ת��ΪDFA" << endl;
        set<int> dfaStates = nfa.states;
        set<char> alphabet = nfa.alphabet;
        map<pair<int, char>, int> transitions;

        // ת��NFA��ÿ��ת�Ƶ�DFA
        for (const auto& transition : nfa.transitions) {
            int fromState = transition.first.first;
            char symbol = transition.first.second;
            int toState = *transition.second.begin(); // ÿ��ת��ֻ��һ��Ŀ��״̬
            transitions[{fromState, symbol}] = toState;
        }

        int startState = nfa.startState;
        set<int> finalStates = nfa.finalStates;

        return DFA(dfaStates, alphabet, transitions, startState, finalStates);
    }

    // �Ӽ����취����NFA��DFA��ת��
    set<set<int>> dfaStates; // DFA��״̬������NFA״̬���Ӽ���ɣ�
    set<char> alphabet = nfa.alphabet;
    map<pair<set<int>, char>, set<int>> dfaTransitions; // DFA��ת�ƺ���

    // �����ʼ״̬�Ħűհ�
    set<int> startState = epsilonClosure({ nfa.startState }, nfa.transitions);
    dfaStates.insert(startState);

    queue<set<int>> stateQueue;
    stateQueue.push(startState);

    set<set<int>> finalStates; // DFA����ֹ״̬��

    // �𲽴���ÿ��״̬
    while (!stateQueue.empty()) {
        set<int> currentState = stateQueue.front();
        stateQueue.pop();

        for (char symbol : alphabet) {
            // ���㵱ǰ״̬���������� symbol �µ�Ŀ��״̬����
            set<int> nextState = move(currentState, symbol, nfa.transitions);

            if (!nextState.empty()) {
                // ��Ŀ��״̬���ϼ���űհ�
                nextState = epsilonClosure(nextState, nfa.transitions);

                // ���Ŀ��״̬��������״̬������DFA��״̬����
                if (!dfaStates.count(nextState)) {
                    dfaStates.insert(nextState);
                    stateQueue.push(nextState);
                }

                // ����DFA��ת�ƺ���
                dfaTransitions[{currentState, symbol}] = nextState;
            }
        }

        // �жϵ�ǰ״̬�����Ƿ�Ϊ��ֹ״̬
        for (int state : currentState) {
            if (nfa.finalStates.count(state)) {
                finalStates.insert(currentState);
                break;
            }
        }
    }

    // ״̬����ӳ��Ϊ����
    map<set<int>, int> stateMapping;
    int stateCount = 0;
    set<int> smalledStates;

    for (const auto& stateSet : dfaStates) {
        stateMapping[stateSet] = stateCount++;
        smalledStates.insert(stateMapping[stateSet]);
    }

    // �µ�ת�ƺ���
    map<pair<int, char>, int> smalledTransitions;
    for (const auto& [key, value] : dfaTransitions) {
        int fromState = stateMapping[key.first];
        int toState = stateMapping[value];
        smalledTransitions[{fromState, key.second}] = toState;
    }

    // ��ƽ����ֹ״̬����
    set<int> smalledFinalStates;
    for (const auto& stateSet : finalStates) {
        smalledFinalStates.insert(stateMapping[stateSet]);
    }

    // ��ƽ����ʼ״̬
    int smalledStartState = stateMapping[startState];

    // ����ȷ�������DFA
    return DFA(smalledStates, alphabet, smalledTransitions, smalledStartState, smalledFinalStates);
}

set<int> epsilonClosure(const set<int>& states, const map<pair<int, char>, set<int>>& transitions) {
    set<int> closure = states; // ��ʼ���հ�Ϊ�����״̬����
    stack<int> stack;          // �����������������ջ

    // �����г�ʼ״̬ѹ��ջ
    for (int state : states) {
        stack.push(state);
    }

    // ���������������������ͨ�� �� ת�ƿ��Ե����״̬
    while (!stack.empty()) {
        int currentState = stack.top();
        stack.pop();

        // ���ҵ�ǰ״̬�� �� ת��
        auto it = transitions.find({ currentState, '-' }); // -1 ��ʾ �� ת��
        if (it != transitions.end()) {
            for (int nextState : it->second) {
                // ����հ�����δ������״̬�������հ���ѹ��ջ
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
    set<int> result; // �洢Ŀ��״̬����

    // ����״̬����
    for (int state : states) {
        // ���ҵ�ǰ״̬��ָ�������µ�ת��
        auto it = transitions.find({ state, symbol });
        if (it != transitions.end()) {
            // ������Ŀ��״̬����������
            result.insert(it->second.begin(), it->second.end());
        }
    }

    return result;
}

DFA mininizedDFA(DFA& dfa) {
    // 1. ��ʼ�����飺��ֹ״̬�ͷ���ֹ״̬
    set<int> finalStates = dfa.finalStates;
    set<int> nonFinalStates;
    bool flag = true;
    for (int state : dfa.states) {
        if (!finalStates.count(state)) {
            nonFinalStates.insert(state);
        }
    }

    // ��ʼ���� P��{��ֹ״̬} �� {����ֹ״̬}
    set<set<int>> partitions;
    partitions.insert(finalStates);
    partitions.insert(nonFinalStates);

    // 2. ��ʼ���ֹ���
    while (flag) {
        flag = false;
        // ���յ�ǰ���ŶԷ������л���
        set<set<int>> newPartitions;
        for (const auto& partition : partitions) {
            map<set<pair<set<int>, char>>, set<int>> veri_mp;   // �洢ͬһ�����set   
            for (auto state : partition) {
                set<pair<set<int>, char>> veri_set; // �洢ת��״̬
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
        // ���·���
        if (newPartitions.size() > partitions.size()) {
            partitions = newPartitions;
            flag = true;
        }
    }

    // 3. ������С�� DFA
    map<set<int>, int> stateMapping; // ����ӳ��ÿ��״̬���鵽��״̬
    int newStateCounter = 0;
    for (const auto& partition : partitions) {
        stateMapping[partition] = newStateCounter++;
    }

    set<int> newStates;
    set<int> newFinalStates;
    map<pair<int, char>, int> newTransitions;
    int newStartState = -1;

    // �����µ�״̬��ת�ƺ�������ʼ״̬����ֹ״̬
    for (const auto& partition : partitions) {
        int newState = stateMapping[partition];
        newStates.insert(newState);

        // ���ԭ���������ʼ״̬�������µĳ�ʼ״̬
        if (partition.count(dfa.startState)) {
            newStartState = newState;
        }

        // ������������ֹ״̬�������µ���ֹ״̬��
        for (int state : partition) {
            if (finalStates.count(state)) {
                newFinalStates.insert(newState);
                break;
            }
        }

        // ����ת�ƺ���
        for (int state : partition) {
            for (char symbol : dfa.alphabet) {
                auto it = dfa.transitions.find({ state, symbol });
                if (it != dfa.transitions.end()) {
                    int targetState = it->second;

                    // �ҵ�Ŀ��״̬�����ķ���
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

    // ������С�� DFA
    return DFA(newStates, dfa.alphabet, newTransitions, newStartState, newFinalStates);
}

const string TXT_PATH = "D:\\study\\רҵ��\\����ԭ��\\Lab2\\NFA1.txt";

int main(int argc, char* argv[]) {
    vector<NFA> nfas;

    if (argc < 2)
    {
        cout << "δ��⵽��������ȡĬ��txt��" << TXT_PATH << endl;
        string s = TXT_PATH;
        NFA nfa = readNFA(s);
        nfas.push_back(nfa);
    }
    else {
        // �����ȡ�������е��ļ�������NFA
        for (int i = 1; i < argc; i++)
        {
            string s = argv[i];
            NFA nfa = readNFA(s);
            nfas.push_back(nfa);
        }
    }
    NFA nfa = dealNFAS(nfas);
    cout << "�ϲ����NFA" << endl;
    printNFA(nfa);

    DFA dfa = transToDFA(nfa);
    cout << endl << "NFAת��DFA" << endl;
    printDFA(dfa);

    dfa = mininizedDFA(dfa);
    cout << endl << "�ϲ����DFA" << endl;
    printDFA(dfa);


    return 0;
}