#pragma once

#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <cctype>
#include <set>
#include <map>
#include <sstream>
#include <fstream>


using namespace std;



struct DFA {
	set<int> states;		// 状态集
	set<char> alphabet;		// 输入字母表
	map<pair<int, char>, int> transitions;		// 状态转换函数	
	int startState;			// 开始状态
	set<int> finalStates;	// 终止状态集
	DFA(set<int> states, set<char> alphabet, map<pair<int, char>, int> transitions,
		int startState, set<int> finalStates) :states(states), alphabet(alphabet), transitions(transitions),
		startState(startState), finalStates(finalStates) {};
	DFA() {};
};

struct NFA {
	set<int> states;	// 有穷状态集
	set<char> alphabet;	// 输入字母表
	map<pair<int, char>, set<int>> transitions;	// 状态转换函数
	int startState;		// 开始状态
	set<int> finalStates;	// 终止状态集
	NFA(set<int> states, set<char> alphabet, map<pair<int, char>, set<int>> transitions,
		int startState, set<int> finalStates) :states(states), alphabet(alphabet), transitions(transitions),
		startState(startState), finalStates(finalStates) {};
	NFA() {};
};


extern NFA turnToNFA(const string& expression);
extern string turnToPostfix(const string& input);
extern void printNFA(NFA& nfa);
extern NFA readNFA(string txt_path);
extern DFA turnToDFA(NFA& nfa);
extern void printDFA(DFA& dfa);
extern DFA mininizedDFA(DFA& dfa);
NFA createBasicNFA(char symbol);
NFA concatNFA(const NFA& nfa1,const NFA& nfa2);
NFA unionNFA(const NFA& nfa1, const NFA& nfa2);
NFA RE_closure(const NFA& nfa);