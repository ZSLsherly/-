#include "help.h"
#include <iomanip>

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
    cout << "��ʼ״̬:" << nfa.startState << endl;
    cout << "��ֹ״̬��:";
    for (const int state : nfa.finalStates)
    {
        cout << state << " ";
    }
    cout << endl;
}

// ������� NFA
NFA createBasicNFA(char symbol) {
    set<int> states = { 0, 1 };
    set<char> alphabet = { symbol };
    map<pair<int, char>, set<int>> transitions = { {{0, symbol}, {1}} };
    return NFA(states, alphabet, transitions, 0, { 1 });
}

// �������� NFA
NFA concatNFA(const NFA& nfa1, const NFA& nfa2) {
    // �ϲ�״̬���ϲ����� nfa2 ��״̬���
    set<int> combinedStates = nfa1.states;
    int offset = *nfa1.states.rbegin() + 1;
    for (int state : nfa2.states) {
        combinedStates.insert(state + offset);
    }

    // �ϲ���ĸ��
    set<char> newAlphabet = nfa1.alphabet;
    newAlphabet.insert(nfa2.alphabet.begin(), nfa2.alphabet.end());

    // �ϲ�ת�ƺ��������� nfa2 ��״̬���
    map<pair<int, char>, set<int>> newTransitions = nfa1.transitions;
    for (const auto& transition : nfa2.transitions) {
        pair<int, char> newKey = { transition.first.first + offset, transition.first.second };
        set<int> adjustedTargets;
        for (int target : transition.second) {
            adjustedTargets.insert(target + offset);
        }
        newTransitions[newKey] = adjustedTargets;
    }

    // ��� �� ת�ƣ�nfa1 ����ֹ״̬���ӵ� nfa2 ����ʼ״̬
    for (int finalState : nfa1.finalStates) {
        newTransitions[{finalState, '-'}].insert(nfa2.startState + offset);
    }

    // ���� nfa2 ����ֹ״̬Ϊ�µ���ֹ״̬����
    set<int> newFinalStates;
    for (int finalState : nfa2.finalStates) {
        newFinalStates.insert(finalState + offset);
    }

    // �������Ӻ�� NFA
    return NFA(combinedStates, newAlphabet, newTransitions, nfa1.startState, newFinalStates);
}

// �ϲ����� NFA��Union��
NFA unionNFA(const NFA& nfa1, const NFA& nfa2) {
    // ��ʼ���ϲ���� NFA
    NFA unionNFA;

    // �µĳ�ʼ״̬
    int newStartState = -1;
    unionNFA.states.insert(newStartState);
    unionNFA.startState = newStartState;

    // �ϲ�״̬����
    for (int state : nfa1.states) {
        unionNFA.states.insert(state);
    }
    for (int state : nfa2.states) {
        unionNFA.states.insert(state);
    }

    // �ϲ���ĸ��
    unionNFA.alphabet.insert(nfa1.alphabet.begin(), nfa1.alphabet.end());
    unionNFA.alphabet.insert(nfa2.alphabet.begin(), nfa2.alphabet.end());

    // �ϲ�״̬ת������
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

    // �ϲ���ֹ״̬��
    unionNFA.finalStates.insert(nfa1.finalStates.begin(), nfa1.finalStates.end());
    unionNFA.finalStates.insert(nfa2.finalStates.begin(), nfa2.finalStates.end());

    // ��� �� ת�ƴ��µĳ�ʼ״̬������ NFA ����ʼ״̬
    unionNFA.transitions[{newStartState, '-'}].insert(nfa1.startState);
    unionNFA.transitions[{newStartState, '-'}].insert(nfa2.startState);

    return unionNFA;
}

// �հ�����
NFA RE_closure(const NFA& nfa) {
    // ����µ���ʼ״̬����ֹ״̬
    set<int> newStates = nfa.states;
    int newStartState = *nfa.states.rbegin() + 1;
    int newFinalState = newStartState + 1;
    newStates.insert(newStartState);
    newStates.insert(newFinalState);

    // ����ԭ����ת�ƺ���
    map<pair<int, char>, set<int>> newTransitions = nfa.transitions;

    // ��� �� ת�ƣ�������ʼ״̬��ԭ��ʼ״̬������ֹ״̬
    newTransitions[{newStartState, '-'}].insert(nfa.startState);
    newTransitions[{newStartState, '-'}].insert(newFinalState);

    // ��� �� ת�ƣ���ԭ��ֹ״̬������ֹ״̬��ԭ��ʼ״̬
    for (int finalState : nfa.finalStates) {
        newTransitions[{finalState, '-'}].insert(newFinalState);
        newTransitions[{finalState, '-'}].insert(nfa.startState);
    }

    // ����Ӧ�� Kleene �ǲ������ NFA
    return NFA(newStates, nfa.alphabet, newTransitions, newStartState, { newFinalState });
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

