#include "help.h"
#include <unordered_map>

map<char, int> operater_num;

// ��ʼ�����������ȼ�
void init_operater_num() {
    operater_num['*'] = 3;
    operater_num['.'] = 2;
    operater_num['|'] = 1;
}

// ��׺���ʽת��Ϊ��׺���ʽ���沨�����ʽ��
string turnToPostfix(const string& input) {
    init_operater_num();
    string temp = "";
    for (size_t i = 0; i < input.length(); i++) {
        temp += input[i];

        if (i + 1 < input.length()) {
            char current = input[i];
            char next = input[i + 1];

            // �����ʽ���ӷ�������
            if ((isalnum(current) || current == ')' || current == '*') &&
                (isalnum(next) || next == '(')) {
                temp += '.';
            }
        }
    }
    stack<char> operatorStack;
    string postfix = "";

    for (char ch : temp) {
        if (isalnum(ch)) {
            postfix += ch;  // ֱ�����������
        }
        else if (ch == '(') {
            operatorStack.push(ch);  // ��������ջ
        }
        else if (ch == ')') {
            // ����ֱ������������
            while (!operatorStack.empty() && operatorStack.top() != '(') {
                postfix += operatorStack.top();
                operatorStack.pop();
            }
            operatorStack.pop();  // ����������
        }
        else if (ch == '|' || ch == '*' || ch == '.' ) {
            // �������ȼ����������
            while (!operatorStack.empty() && operater_num[operatorStack.top()] >= operater_num[ch]) {
                postfix += operatorStack.top();
                operatorStack.pop();
            }
            operatorStack.push(ch);
        }
    }

    // ����ջ��ʣ��Ĳ�����
    while (!operatorStack.empty()) {
        postfix += operatorStack.top();
        operatorStack.pop();
    }

    return postfix;
}

// �Ӻ�׺���ʽ���� NFA
NFA turnToNFA(const string& postfix) {
    stack<NFA> nfaStack;

    for (char token : postfix) {
        if (isalnum(token)) {
            nfaStack.push(createBasicNFA(token));  // ������ NFA
        }
        else if (token == '*') {
            // ���ֱհ�
            NFA nfa = nfaStack.top(); nfaStack.pop();
            nfaStack.push(RE_closure(nfa));
        }
        else if (token == '.') {
            // ���Ӳ���
            NFA nfa2 = nfaStack.top(); nfaStack.pop();
            NFA nfa1 = nfaStack.top(); nfaStack.pop();
            nfaStack.push(concatNFA(nfa1, nfa2));
        }
        else if (token == '|') {
            // ��������
            NFA nfa2 = nfaStack.top(); nfaStack.pop();
            NFA nfa1 = nfaStack.top(); nfaStack.pop();
            nfaStack.push(unionNFA(nfa1, nfa2));
        }
    }
    return nfaStack.top();  // ���յ� NFA
}
