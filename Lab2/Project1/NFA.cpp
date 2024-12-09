//#include "help.h"
//#include <iomanip>
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
//    cout << "��ʼ״̬:" << nfa.startState << endl;
//    cout << "��ֹ״̬��:";
//    for (const int state : nfa.finalStates)
//    {
//        cout << state << " ";
//    }
//    cout << endl;
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
//// �ϲ����nfa
//NFA dealNFAS(vector<NFA>& nfas)
//{
//    if (nfas.empty())
//    {
//        cout << "û��NFA���Ժϲ���" << endl;
//        exit(1);
//    }
//    // ��ʼ���ϲ����NFA
//    NFA nfa;
//    // �µĳ�ʼ״̬
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
//        // �ϲ���ĸ��
//        nfa.alphabet.insert(nfa_temp.alphabet.begin(), nfa_temp.alphabet.end());
//
//        // �ϲ�״̬ת������
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
//        // �ϲ���ֹ״̬��
//        for (int finalState : nfa_temp.finalStates)
//        {
//            nfa.finalStates.insert(finalState);
//        }
//
//        // ��Ӧ�ת�Ƶ�ԭʼNFA����ʼ״̬
//        nfa.transitions[{newStartState, '-'}].insert(nfa_temp.startState);
//
//    }
//
//    return nfa;
//}
