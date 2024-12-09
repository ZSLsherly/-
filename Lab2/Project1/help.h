//#pragma once
//
//#include <iostream>
//#include <set>
//#include <map>
//#include <vector>
//#include <string>
//#include <sstream>
//#include <fstream>
//
//using namespace std;
//
//struct DFA {
//	set<int> states;		// ״̬��
//	set<char> alphabet;		// ������ĸ��
//	map<pair<int, char>, int> transitions;		// ״̬ת������	
//	int startState;			// ��ʼ״̬
//	set<int> finalStates;	// ��ֹ״̬��
//	DFA(set<int> states, set<char> alphabet, map<pair<int, char>, int> transitions,
//		int startState, set<int> finalStates) :states(states), alphabet(alphabet), transitions(transitions),
//		startState(startState), finalStates(finalStates) {};
//};
//
//struct NFA {
//	set<int> states;	// ����״̬��
//	set<char> alphabet;	// ������ĸ��
//	map<pair<int, char>, set<int>> transitions;	// ״̬ת������
//	int startState;		// ��ʼ״̬
//	set<int> finalStates;	// ��ֹ״̬��
//	NFA(set<int> states, set<char> alphabet, map<pair<int, char>, set<int>> transitions,
//		int startState, set<int> finalStates) :states(states), alphabet(alphabet), transitions(transitions),
//		startState(startState), finalStates(finalStates) {};
//	NFA(){};
//};
//
//extern void printNFA(NFA& nfa);
//extern NFA readNFA(string txt_path);
//NFA dealNFAS(vector<NFA>& nfas);
//extern DFA transToDFA(NFA& nfa);
//extern void printDFA(DFA& dfa);
//extern DFA mininizedDFA(DFA& dfa);