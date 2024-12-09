#include "help.h"
#include <unordered_map>

map<char, int> operater_num;

// 初始化操作符优先级
void init_operater_num() {
    operater_num['*'] = 3;
    operater_num['.'] = 2;
    operater_num['|'] = 1;
}

// 中缀表达式转换为后缀表达式（逆波兰表达式）
string turnToPostfix(const string& input) {
    init_operater_num();
    string temp = "";
    for (size_t i = 0; i < input.length(); i++) {
        temp += input[i];

        if (i + 1 < input.length()) {
            char current = input[i];
            char next = input[i + 1];

            // 添加显式连接符的条件
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
            postfix += ch;  // 直接输出操作数
        }
        else if (ch == '(') {
            operatorStack.push(ch);  // 左括号入栈
        }
        else if (ch == ')') {
            // 弹出直到遇到左括号
            while (!operatorStack.empty() && operatorStack.top() != '(') {
                postfix += operatorStack.top();
                operatorStack.pop();
            }
            operatorStack.pop();  // 弹出左括号
        }
        else if (ch == '|' || ch == '*' || ch == '.' ) {
            // 根据优先级处理操作符
            while (!operatorStack.empty() && operater_num[operatorStack.top()] >= operater_num[ch]) {
                postfix += operatorStack.top();
                operatorStack.pop();
            }
            operatorStack.push(ch);
        }
    }

    // 处理栈中剩余的操作符
    while (!operatorStack.empty()) {
        postfix += operatorStack.top();
        operatorStack.pop();
    }

    return postfix;
}

// 从后缀表达式生成 NFA
NFA turnToNFA(const string& postfix) {
    stack<NFA> nfaStack;

    for (char token : postfix) {
        if (isalnum(token)) {
            nfaStack.push(createBasicNFA(token));  // 基本的 NFA
        }
        else if (token == '*') {
            // 克林闭包
            NFA nfa = nfaStack.top(); nfaStack.pop();
            nfaStack.push(RE_closure(nfa));
        }
        else if (token == '.') {
            // 连接操作
            NFA nfa2 = nfaStack.top(); nfaStack.pop();
            NFA nfa1 = nfaStack.top(); nfaStack.pop();
            nfaStack.push(concatNFA(nfa1, nfa2));
        }
        else if (token == '|') {
            // 并集操作
            NFA nfa2 = nfaStack.top(); nfaStack.pop();
            NFA nfa1 = nfaStack.top(); nfaStack.pop();
            nfaStack.push(unionNFA(nfa1, nfa2));
        }
    }
    return nfaStack.top();  // 最终的 NFA
}
