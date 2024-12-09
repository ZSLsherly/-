#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <string>
#include <sstream>
#include <iomanip>
#include <tuple>

using namespace std;

// 定义产生式结构
struct Production {
    string left;
    vector<string> right;
};

// 定义项目结构
struct Item {
    int prodIndex;
    int dotPosition;

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

    Grammar(){};
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
    map<pair<int, string>, int> transitions;
    map<pair<int, string>, string> actionTable;
    map<pair<int, string>, int> gotoTable;   

    LR0Parser(Grammar grammar) : grammar(grammar) {}

    void createDFA();
    void createTables();
    void printDFA();
    void printTables(const string& filename);

private:
    set<Item> closure(const set<Item>& items);
    int findItemSet(const set<Item>& items);
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


// 查找项目集编号
int LR0Parser::findItemSet(const set<Item>& items) {
    for (int i = 0; i < itemSets.size(); ++i) {
        if (itemSets[i] == items) {
            return i;
        }
    }
    return -1;
}

// 计算闭包
set<Item> LR0Parser::closure(const set<Item>& items) {
    set<Item> closureSet = items;
    queue<Item> q;
    for (const auto& item : items) q.push(item);

    while (!q.empty()) {
        Item item = q.front();
        q.pop();

        // 检查点后是否还有符号
        if (item.dotPosition < grammar.productions[item.prodIndex].right.size()) {
            string symbol = grammar.productions[item.prodIndex].right[item.dotPosition];
            if (grammar.nonTerminals.count(symbol)) {
                // 扩展非终结符
                for (int i = 0; i < grammar.productions.size(); ++i) {
                    if (grammar.productions[i].left == symbol) {
                        Item newItem = { i, 0 };
                        closureSet.insert(newItem);
                    }
                }
            }
        }
    }

    return closureSet;
}

// 构造 DFA
void LR0Parser::createDFA() {
    set<Item> initialSet = closure({ {0, 0} }); // 初始项目集
    itemSets.push_back(initialSet);

    queue<int> q;
    q.push(0);

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        // 计算转移
        map<string, set<Item>> moves;
        for (const auto& item : itemSets[current]) {
            if (item.dotPosition < grammar.productions[item.prodIndex].right.size()) {
                string symbol = grammar.productions[item.prodIndex].right[item.dotPosition];
                Item newItem = item;
                newItem.dotPosition++;
                moves[symbol].insert(newItem);
            }
        }

        // 创建新项目集
        for (const auto& move : moves) {
            set<Item> newSet = closure(move.second);
            int index = findItemSet(newSet);
            if (index == -1) {
                index = itemSets.size();
                itemSets.push_back(newSet);
                q.push(index);
            }
            transitions[{current, move.first}] = index;
        }
    }
}

// 构造分析表
void LR0Parser::createTables() {
    for (int state = 0; state < itemSets.size(); ++state) {
        for (const auto& item : itemSets[state]) {
            if (item.dotPosition == grammar.productions[item.prodIndex].right.size()) {
                // 点在末尾，规约或接受
                if (grammar.productions[item.prodIndex].left == grammar.startSymbol) {
                    actionTable[{state, "#"}] = "acc"; // 接受状态
                }
                else {
                    for (const auto& terminal : grammar.terminals) {
                        actionTable[{state, terminal}] = "r" + to_string(item.prodIndex);
                    }
                    actionTable[{state, "#"}] = "r" + to_string(item.prodIndex);
                }
            }
            else {
                // 点未到末尾，移进
                string symbol = grammar.productions[item.prodIndex].right[item.dotPosition];
                if (grammar.terminals.count(symbol)) {
                    int targetState = transitions[{state, symbol}];
                    actionTable[{state, symbol}] = "s" + to_string(targetState);
                }
            }
        }

        // Goto 表
        for (const auto& nonTerminal : grammar.nonTerminals) {
            if (transitions.count({ state, nonTerminal })) {
                gotoTable[{state, nonTerminal}] = transitions[{state, nonTerminal}];
            }
        }
    }
}

// 输出项目集族
void LR0Parser::printDFA() {
    cout << "[LR(0) 项目集族]" << endl;
    for (int i = 0; i < itemSets.size(); ++i) {
        cout << "I" << i << " :" << endl;
        for (const auto& item : itemSets[i]) {
            const auto& prod = grammar.productions[item.prodIndex];
            cout << "  " << prod.left << " -> ";
            for (int j = 0; j < prod.right.size(); ++j) {
                if (j == item.dotPosition) cout << ".";
                cout << prod.right[j];
            }
            if (item.dotPosition == prod.right.size()) cout << ".";
            cout << endl;
        }
    }

    cout << "[LR(0) 状态转移]" << endl;
    for (const auto& transition : transitions) {
        cout << transition.first.first << " , " << transition.first.second
            << " -> " << transition.second << endl;
    }
}

// 输出分析表到文件
void LR0Parser::printTables(const string& filename) {
    ofstream outfile(filename);
    if (!outfile.is_open()) {
        cerr << "无法创建分析表文件！" << endl;
        exit(1);
    }

    // 输出记录总数
    outfile << actionTable.size() << endl;

    // 输出 Action 表
    for (const auto& action : actionTable) {
        outfile << action.first.first << " " << action.first.second << " " << action.second << endl;
    }
    outfile << gotoTable.size() << endl;

    // 输出 Goto 表
    for (const auto& gt : gotoTable) {
        outfile << gt.first.first << " " << gt.first.second << " " << gt.second << endl;
    }

    outfile.close();
}

// 主函数
int main(int argc, char* argv[]) {
    // 输入文件和输出文件路径
    string grammarFile = (argc < 2) ? "D:\\study\\专业课\\编译原理\\Lab7\\test.txt" : argv[1];
    cout << "读入文件为:" << grammarFile << endl;
    string tableFile = grammarFile.substr(0, grammarFile.length() - 3) + "lrtbl";
    cout << "结果将输出至:" << tableFile << endl;

    Grammar grammar(grammarFile);

    LR0Parser parser(grammar);

    parser.createDFA();
    parser.printDFA();
    parser.createTables();
    parser.printTables(tableFile);

    cout << "LR(0) 分析表已生成，文件: " << tableFile << endl;
    return 0;
}
