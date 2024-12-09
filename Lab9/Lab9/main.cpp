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

// �������ʽ�ṹ
struct Production {
    string left;                 // ����ʽ��
    vector<string> right;        // ����ʽ�Ҳ�
};

// ������Ŀ�ṹ
struct Item {
    int prodIndex;               // ��Ŀ��Ӧ�Ĳ���ʽ���
    int dotPosition;             // ���λ��

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

    Grammar() {};
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
    map<pair<int, string>, string> actionTable;
    map<pair<int, string>, int> gotoTable;

    LR0Parser(Grammar grammar) : grammar(grammar) {}

    void readTable(const string& tableFile);
    void runParser(const vector<string>& input);
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

// ��ȡ�������ļ�
void LR0Parser::readTable(const string& tableFile) {
    ifstream infile(tableFile);
    if (!infile.is_open()) {
        cerr << "�޷��򿪷������ļ���" << endl;
        exit(1);
    }

    int actionCount;
    infile >> actionCount;

    // ��ȡAction��
    for (int i = 0; i < actionCount; ++i) {
        int state;
        string symbol, action;
        infile >> state >> symbol >> action;
        actionTable[{state, symbol}] = action;
    }

    int gotoCount;
    infile >> gotoCount;

    // ��ȡGoto��
    for (int i = 0; i < gotoCount; ++i) {
        int state;
        string nonTerminal;
        int targetState;
        infile >> state >> nonTerminal >> targetState;
        gotoTable[{state, nonTerminal}] = targetState;
    }

    infile.close();
}

// ִ���﷨����
void LR0Parser::runParser(const vector<string>& input) {
    stack<int> stateStack;
    stack<string> symbolStack;
    queue<string> inputWords;
    // ��ʼ��
    stateStack.push(0);
    symbolStack.push("#");
    for (auto it : input) {
        inputWords.push(it);
    }
    inputWords.push("#");

    cout << "[parsing]" << endl;
    cout << left << setw(8) << "ջ��" << setw(8) << "����" << setw(8) << "���" << setw(8) << "����" << endl;


    while (true) {
        int currentState = stateStack.top();
        string currentInput = inputWords.front();
        string action = actionTable[{currentState, currentInput}];

        cout << setw(8) << stateStack.top() << setw(8) << currentInput << setw(8) << action;

        if (action == "acc") {
            cout << "�ɹ����գ�" << endl;
            break;
        }
        else if (action[0] == 's') {            // �ƽ�
            int nextState = stoi(action.substr(1));
            stateStack.push(nextState);
            symbolStack.push(currentInput);
            inputWords.pop();
            cout << "��ջ " << nextState << " " << currentInput << endl;
        }
        else if (action[0] == 'r') {            // ��Լ
            int prodIndex = stoi(action.substr(1));
            const Production& prod = grammar.productions[prodIndex];
            for (int i = 0; i < prod.right.size(); ++i) {
                stateStack.pop();
                symbolStack.pop();
            }
            int gotoState = gotoTable[{stateStack.top(), prod.left}];
            stateStack.push(gotoState);
            symbolStack.push(prod.left);
            cout << "��ջ" << prod.right.size() << "�����ź�״̬ " << "��ջ " << gotoState << " " << setw(8) << prod.left << prod.left << "->";
            for (auto it : prod.right) {
                cout << it;
            }
            cout << endl;
        }
        else {
            cerr << "����δ֪�Ķ�����" << endl;
            break;
        }
    }
}

// ��ȡ�����ַ�����
vector<string> read_input(string filename) {
    string line;
    vector<string> symbols;
    ifstream infile(filename);

    if (!infile.is_open()) {
        cerr << "�޷����ļ� " << filename << endl;
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
    string grammarFile = (argc < 2) ? "D:\\study\\רҵ��\\����ԭ��\\Lab9\\test.txt" : argv[1];
    string tableFile = (argc < 3) ? "D:\\study\\רҵ��\\����ԭ��\\Lab9\\test.lrtbl" : argv[2];
    string inputFile = (argc < 4) ? "D:\\study\\רҵ��\\����ԭ��\\Lab9\\test_input.txt" : argv[3];

    // ��������
    Grammar grammar(grammarFile);
    LR0Parser parser(grammar);
    parser.readTable(tableFile);
    vector<string> input = read_input(inputFile);

    parser.runParser(input);
    return 0;
}