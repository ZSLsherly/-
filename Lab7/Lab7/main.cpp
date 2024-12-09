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

// �������ʽ�ṹ
struct Production {
    string left;
    vector<string> right;
};

// ������Ŀ�ṹ
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

// �ķ���
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

// LR��
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

// ���ļ������ķ�
void Grammar::read(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "�޷����ķ������ļ���" << endl;
        exit(1);
    }

    int vnCount, vtCount, pCount;

    // ��ȡ���ս��
    file >> vnCount;
    for (int i = 0; i < vnCount; ++i) {
        string vn;
        file >> vn;
        nonTerminals.insert(vn);
    }

    // ��ȡ�ս��
    file >> vtCount;
    for (int i = 0; i < vtCount; ++i) {
        string vt;
        file >> vt;
        terminals.insert(vt);
    }

    // ��ȡ����ʽ
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

    // ��ȡ��ʼ����
    file >> startSymbol;
    file.close();
}


// ������Ŀ�����
int LR0Parser::findItemSet(const set<Item>& items) {
    for (int i = 0; i < itemSets.size(); ++i) {
        if (itemSets[i] == items) {
            return i;
        }
    }
    return -1;
}

// ����հ�
set<Item> LR0Parser::closure(const set<Item>& items) {
    set<Item> closureSet = items;
    queue<Item> q;
    for (const auto& item : items) q.push(item);

    while (!q.empty()) {
        Item item = q.front();
        q.pop();

        // ������Ƿ��з���
        if (item.dotPosition < grammar.productions[item.prodIndex].right.size()) {
            string symbol = grammar.productions[item.prodIndex].right[item.dotPosition];
            if (grammar.nonTerminals.count(symbol)) {
                // ��չ���ս��
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

// ���� DFA
void LR0Parser::createDFA() {
    set<Item> initialSet = closure({ {0, 0} }); // ��ʼ��Ŀ��
    itemSets.push_back(initialSet);

    queue<int> q;
    q.push(0);

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        // ����ת��
        map<string, set<Item>> moves;
        for (const auto& item : itemSets[current]) {
            if (item.dotPosition < grammar.productions[item.prodIndex].right.size()) {
                string symbol = grammar.productions[item.prodIndex].right[item.dotPosition];
                Item newItem = item;
                newItem.dotPosition++;
                moves[symbol].insert(newItem);
            }
        }

        // ��������Ŀ��
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

// ���������
void LR0Parser::createTables() {
    for (int state = 0; state < itemSets.size(); ++state) {
        for (const auto& item : itemSets[state]) {
            if (item.dotPosition == grammar.productions[item.prodIndex].right.size()) {
                // ����ĩβ����Լ�����
                if (grammar.productions[item.prodIndex].left == grammar.startSymbol) {
                    actionTable[{state, "#"}] = "acc"; // ����״̬
                }
                else {
                    for (const auto& terminal : grammar.terminals) {
                        actionTable[{state, terminal}] = "r" + to_string(item.prodIndex);
                    }
                    actionTable[{state, "#"}] = "r" + to_string(item.prodIndex);
                }
            }
            else {
                // ��δ��ĩβ���ƽ�
                string symbol = grammar.productions[item.prodIndex].right[item.dotPosition];
                if (grammar.terminals.count(symbol)) {
                    int targetState = transitions[{state, symbol}];
                    actionTable[{state, symbol}] = "s" + to_string(targetState);
                }
            }
        }

        // Goto ��
        for (const auto& nonTerminal : grammar.nonTerminals) {
            if (transitions.count({ state, nonTerminal })) {
                gotoTable[{state, nonTerminal}] = transitions[{state, nonTerminal}];
            }
        }
    }
}

// �����Ŀ����
void LR0Parser::printDFA() {
    cout << "[LR(0) ��Ŀ����]" << endl;
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

    cout << "[LR(0) ״̬ת��]" << endl;
    for (const auto& transition : transitions) {
        cout << transition.first.first << " , " << transition.first.second
            << " -> " << transition.second << endl;
    }
}

// ����������ļ�
void LR0Parser::printTables(const string& filename) {
    ofstream outfile(filename);
    if (!outfile.is_open()) {
        cerr << "�޷������������ļ���" << endl;
        exit(1);
    }

    // �����¼����
    outfile << actionTable.size() << endl;

    // ��� Action ��
    for (const auto& action : actionTable) {
        outfile << action.first.first << " " << action.first.second << " " << action.second << endl;
    }
    outfile << gotoTable.size() << endl;

    // ��� Goto ��
    for (const auto& gt : gotoTable) {
        outfile << gt.first.first << " " << gt.first.second << " " << gt.second << endl;
    }

    outfile.close();
}

// ������
int main(int argc, char* argv[]) {
    // �����ļ�������ļ�·��
    string grammarFile = (argc < 2) ? "D:\\study\\רҵ��\\����ԭ��\\Lab7\\test.txt" : argv[1];
    cout << "�����ļ�Ϊ:" << grammarFile << endl;
    string tableFile = grammarFile.substr(0, grammarFile.length() - 3) + "lrtbl";
    cout << "����������:" << tableFile << endl;

    Grammar grammar(grammarFile);

    LR0Parser parser(grammar);

    parser.createDFA();
    parser.printDFA();
    parser.createTables();
    parser.printTables(tableFile);

    cout << "LR(0) �����������ɣ��ļ�: " << tableFile << endl;
    return 0;
}
