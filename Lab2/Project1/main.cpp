//#include "help.h"
//
//const string TXT_PATH = "D:\\study\\专业课\\编译原理\\Lab2\\NFA1.txt";
//
//int main(int argc,char* argv[]) {
//    vector<NFA> nfas;
//
//    if (argc < 2)
//    {
//        cout << "未检测到参数，读取默认txt：" << TXT_PATH << endl;
//        string s = TXT_PATH;
//        NFA nfa = readNFA(s);
//        nfas.push_back(nfa);
//    }
//    else {
//        // 逐个读取命令行中的文件，构造NFA
//        for (int i = 1; i < argc; i++)
//        {
//            string s = argv[i];
//            NFA nfa = readNFA(s);
//            nfas.push_back(nfa);
//        }
//    }
//    NFA nfa = dealNFAS(nfas);
//    cout << "合并后的NFA" << endl;
//    printNFA(nfa);
//
//    DFA dfa = transToDFA(nfa);
//    cout << endl << "NFA转成DFA" << endl;
//    printDFA(dfa);
//
//    dfa = mininizedDFA(dfa);
//    cout << endl << "合并后的DFA" << endl;
//    printDFA(dfa);
//
//
//	return 0;
//}