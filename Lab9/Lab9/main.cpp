#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <queue>
#include <sstream>
#include <iomanip>
#include <stack>
#include <tuple>

using namespace std;

// 定义产生式结构
struct Production {
    string left;                 // 产生式左部
    vector<string> right;        // 产生式右部
};

// 定义项目结构
struct Item {
    int prodIndex;               // 项目对应的产生式编号
    int dotPosition;             // 点的位置

    bool operator==(const Item& other) const {
        return prodIndex == other.prodIndex && dotPosition == other.dotPosition;
    }

    bool operator<(const Item& other) const {
        return tie(prodIndex, dotPosition) < tie(other.prodIndex, other.dotPosition);
    }
};

// 文法类
class Grammar {
public:
    vector<Production> productions;
    set<string> nonTerminals;
    set<string> terminals;
    string startSymbol;

    Grammar() {};
    Grammar(string fileName) {
        read(fileName);
    }

    void read(const string& fileName);
};

// LR类
class LR0Parser {
public:
    Grammar grammar;
    vector<set<Item>> itemSets;
    map<pair<int, string>, string> actionTable;
    map<pair<int, string>, int> gotoTable;

    LR0Parser(Grammar grammar) : grammar(grammar) {}

    void readTable(const string& tableFile);
    void runParser(const vector<string>& input);
};

// 从文件加载文法
void Grammar::read(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "无法打开文法描述文件！" << endl;
        exit(1);
    }

    int vnCount, vtCount, pCount;

    // 读取非终结符
    file >> vnCount;
    for (int i = 0; i < vnCount; ++i) {
        string vn;
        file >> vn;
        nonTerminals.insert(vn);
    }

    // 读取终结符
    file >> vtCount;
    for (int i = 0; i < vtCount; ++i) {
        string vt;
        file >> vt;
        terminals.insert(vt);
    }

    // 读取产生式
    file >> pCount;
    for (int i = 0; i < pCount; ++i) {
        string left, arrow, right;
        file >> left >> arrow;
        getline(file, right);
        vector<string> rhs;
        istringstream iss(right);
        string symbol;
        while (iss >> symbol) {
            rhs.push_back(symbol);
        }
        productions.push_back({ left, rhs });
    }

    // 读取起始符号
    file >> startSymbol;
    file.close();
}

// 读取分析表文件
void LR0Parser::readTable(const string& tableFile) {
    ifstream infile(tableFile);
    if (!infile.is_open()) {
        cerr << "无法打开分析表文件！" << endl;
        exit(1);
    }

    int actionCount;
    infile >> actionCount;

    // 读取Action表
    for (int i = 0; i < actionCount; ++i) {
        int state;
        string symbol, action;
        infile >> state >> symbol >> action;
        actionTable[{state, symbol}] = action;
    }

    int gotoCount;
    infile >> gotoCount;

    // 读取Goto表
    for (int i = 0; i < gotoCount; ++i) {
        int state;
        string nonTerminal;
        int targetState;
        infile >> state >> nonTerminal >> targetState;
        gotoTable[{state, nonTerminal}] = targetState;
    }

    infile.close();
}

// 执行语法分析
void LR0Parser::runParser(const vector<string>& input) {
    stack<int> stateStack;
    stack<string> symbolStack;
    queue<string> inputWords;
    // 初始化
    stateStack.push(0);
    symbolStack.push("#");
    for (auto it : input) {
        inputWords.push(it);
    }
    inputWords.push("#");

    cout << "[parsing]" << endl;
    cout << left << setw(8) << "栈顶" << setw(8) << "输入" << setw(8) << "查表" << setw(8) << "动作" << endl;


    while (true) {
        int currentState = stateStack.top();
        string currentInput = inputWords.front();
        string action = actionTable[{currentState, currentInput}];

        cout << setw(8) << stateStack.top() << setw(8) << currentInput << setw(8) << action;

        if (action == "acc") {
            cout << "成功接收！" << endl;
            break;
        }
        else if (action[0] == 's') {            // 移进
            int nextState = stoi(action.substr(1));
            stateStack.push(nextState);
            symbolStack.push(currentInput);
            inputWords.pop();
            cout << "进栈 " << nextState << " " << currentInput << endl;
        }
        else if (action[0] == 'r') {            // 规约
            int prodIndex = stoi(action.substr(1));
            const Production& prod = grammar.productions[prodIndex];
            for (int i = 0; i < prod.right.size(); ++i) {
                stateStack.pop();
                symbolStack.pop();
            }
            int gotoState = gotoTable[{stateStack.top(), prod.left}];
            stateStack.push(gotoState);
            symbolStack.push(prod.left);
            cout << "出栈" << prod.right.size() << "个符号和状态 " << "进栈 " << gotoState << " " << setw(8) << prod.left << prod.left << "->";
            for (auto it : prod.right) {
                cout << it;
            }
            cout << endl;
        }
        else {
            cerr << "错误：未知的动作！" << endl;
            break;
        }
    }
}

// 读取输入字符串组
vector<string> read_input(string filename) {
    string line;
    vector<string> symbols;
    ifstream infile(filename);

    if (!infile.is_open()) {
        cerr << "无法打开文件 " << filename << endl;
        return symbols;
    }

    while (getline(infile, line)) {
        stringstream ss(line);
        string symbol;

        while (ss >> symbol) {
            symbols.push_back(symbol);
        }
    }

    infile.close();
    return symbols;
}

int main(int argc, char* argv[]) {
    string grammarFile = (argc < 2) ? "D:\\study\\专业课\\编译原理\\Lab9\\test.txt" : argv[1];
    string tableFile = (argc < 3) ? "D:\\study\\专业课\\编译原理\\Lab9\\test.lrtbl" : argv[2];
    string inputFile = (argc < 4) ? "D:\\study\\专业课\\编译原理\\Lab9\\test_input.txt" : argv[3];

    // 基础读入
    Grammar grammar(grammarFile);
    LR0Parser parser(grammar);
    parser.readTable(tableFile);
    vector<string> input = read_input(inputFile);

    parser.runParser(input);
    return 0;
}