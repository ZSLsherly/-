//#include "help.h"
//
//const string TXT_PATH = "D:\\study\\רҵ��\\����ԭ��\\Lab2\\NFA1.txt";
//
//int main(int argc,char* argv[]) {
//    vector<NFA> nfas;
//
//    if (argc < 2)
//    {
//        cout << "δ��⵽��������ȡĬ��txt��" << TXT_PATH << endl;
//        string s = TXT_PATH;
//        NFA nfa = readNFA(s);
//        nfas.push_back(nfa);
//    }
//    else {
//        // �����ȡ�������е��ļ�������NFA
//        for (int i = 1; i < argc; i++)
//        {
//            string s = argv[i];
//            NFA nfa = readNFA(s);
//            nfas.push_back(nfa);
//        }
//    }
//    NFA nfa = dealNFAS(nfas);
//    cout << "�ϲ����NFA" << endl;
//    printNFA(nfa);
//
//    DFA dfa = transToDFA(nfa);
//    cout << endl << "NFAת��DFA" << endl;
//    printDFA(dfa);
//
//    dfa = mininizedDFA(dfa);
//    cout << endl << "�ϲ����DFA" << endl;
//    printDFA(dfa);
//
//
//	return 0;
//}