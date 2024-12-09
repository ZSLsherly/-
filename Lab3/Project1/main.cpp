#include "help.h"

int main(int argc,char* argv[]) {
	if (argc < 2)
	{
		cerr << "δ����RE" << endl;
		return 1;
	}

	string RE = argv[1];
	cout << "���������ʽ:" << endl;
	cout << RE << endl;

	string postfix = turnToPostfix(RE);
	cout << "ת����ĺ�׺���ʽ" << endl;
	cout << postfix << endl;

	NFA nfa = turnToNFA(postfix);
	cout << "ת�����NFA:" << endl;
	printNFA(nfa);

	DFA dfa = turnToDFA(nfa);
	cout << endl << "NFAת��DFA" << endl;
	printDFA(dfa);

	dfa = mininizedDFA(dfa);
	cout << endl << "��С�����DFA" << endl;
	printDFA(dfa);

	return 0;
}

