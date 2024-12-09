#include "help.h"

int main(int argc,char* argv[]) {
	if (argc < 2)
	{
		cerr << "未输入RE" << endl;
		return 1;
	}

	string RE = argv[1];
	cout << "输入的正规式:" << endl;
	cout << RE << endl;

	string postfix = turnToPostfix(RE);
	cout << "转化后的后缀表达式" << endl;
	cout << postfix << endl;

	NFA nfa = turnToNFA(postfix);
	cout << "转换后的NFA:" << endl;
	printNFA(nfa);

	DFA dfa = turnToDFA(nfa);
	cout << endl << "NFA转成DFA" << endl;
	printDFA(dfa);

	dfa = mininizedDFA(dfa);
	cout << endl << "最小化后的DFA" << endl;
	printDFA(dfa);

	return 0;
}

