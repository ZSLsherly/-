#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stack>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <algorithm>
#include<iomanip>

using namespace std;

class Grammar {
public:
    set<string> nonTerminals;    // ���ս������
    set<string> terminals;       // �ս������
    map<string, set<vector<string>>> productions;  // ����ʽ���ϣ�key�Ƿ��ս����value�ǲ���ʽ�Ҳ��ļ���
    string startSymbol;             // ��ʼ����
    map<string, set<string>> firstSet;
    map<string, set<string>> followSet;
    map<pair<string, vector<string>>, set<string>> selectSet;
    map<pair<string, string>, vector<string>> table;

    void readGrammar(const string& fileName);
    void printGrammar();
    void getFirst();
    void getFollow();
    void getSelect();
    void getTable();
    void analyseStr(vector<string> str);
    // ���캯������ȡ�ķ��ļ�
    Grammar(const string& fileName) {
        readGrammar(fileName);
    }

};

// ��ȡ�ķ������ļ�
void Grammar::readGrammar(const string& fileName) {
    ifstream file(fileName);
    if (!file) {
        cerr << "Error opening file: " << fileName << std::endl;
        return;
    }
    int numNonTerminals, numTerminals, numProductions;

    // ��ȡ���ս��
    file >> numNonTerminals;
    for (int i = 0; i < numNonTerminals; i++)
    {
        string temp;
        file >> temp;
        nonTerminals.insert(temp);
    }

    // ��ȡ�ս��
    file >> numTerminals;
    for (int i = 0; i < numTerminals; i++)
    {
        string temp;
        file >> temp;
        terminals.insert(temp);
    }

    // ��ȡ����ʽ
    file >> numProductions;
    file.get(); // ��ȡʣ��Ļ��з�
    for (int i = 0; i < numProductions; i++)
    {
        string line;
        getline(file, line);    // ��ȡһ���У���������
        istringstream iss(line);    // ����ת��Ϊ�����ַ���
        string left, arrow, symbol;
        vector<string>right;

        iss >> left >> arrow;   // ��ȡ�󲿺ͼ�ͷ
        while (iss >> symbol)
        {
            if (symbol == "ε") symbol = "��";    // ��������һ�±��뼯���µĦű��ε������
            right.push_back(symbol);
        }

        productions[left].insert(right);
    }

    // ��ȡ��ʼ��
    file >> startSymbol;
    file.close();
}

void Grammar::getFirst() {
    bool flag = true;
    //map<string, set<vector<string>>> productions;
    // set<string> nonTerminals;    // ���ս������
    //set<string> terminals;
    map<string, set<string>> firstTempSet;  //���ڼ�¼first�ķ��ս��
    for (auto left : nonTerminals) {
        for (auto vect : productions[left]) {
            bool veri = false;
            for (auto now : vect) {
                if (terminals.find(now) != terminals.end() || now == "��") {
                    firstSet[left].insert(now);
                }
                else {
                    for (auto it : productions[now]) {
                        if (it[0] == "��") {
                            veri = true;
                        }
                    }
                    firstTempSet[left].insert(now);
                }
                if (!veri)
                    break;
            }
        }
    }
    while (flag) {
        flag = false;
        for (auto left : nonTerminals) {
            int size = firstSet[left].size();
            for (auto temp : firstTempSet[left]) {
                for (auto it : firstSet[temp]) {
                    if (it != "��") firstSet[left].insert(it);
                }
            }
            if (size < firstSet[left].size())
                flag = true;
        }
    }
}

// ���ɲ������Ӧ��Ԥ���
void Grammar::getTable() {
    // ���� selectSet����������
    for (const auto& entry : selectSet) {
        const string& nonTerminal = entry.first.first;   // ���ս��
        const vector<string>& production = entry.first.second; // ����ʽ
        const set<string>& select = entry.second;       // select ��

        // ��ÿ�� select ����������
        for (const auto& terminal : select) {
            table[{nonTerminal, terminal}] = production;
        }
    }
    cout << "[Ԥ���]" << endl;
    cout << setw(15) << "���ս��\\�ս��";

    for (const auto& terminal : terminals) {
        cout << setw(12) << terminal;
    }
    cout << setw(12) << "#" << endl;

    // �����������
    for (const auto& nonTerminal : nonTerminals) {
        cout << setw(15) << nonTerminal;  // ��ӡ���ս��

        // ���� terminals
        for (const auto& terminal : terminals) {
            auto it = table.find({ nonTerminal, terminal });
            if (it != table.end()) {
                // ������ʽת��Ϊ�ַ�����ʽ�����ڴ洢�ڷ������У�
                string productionStr;
                productionStr += nonTerminal + "->";
                for (const auto& symbol : it->second) {
                    productionStr += symbol;
                }
                cout << setw(12) << productionStr;
            }
            else {
                cout << setw(12) << " ";
            }
        }
        cout << endl;
    }
}

void Grammar::analyseStr(vector<string> str) {
    stack<string> stackStr;
    queue<string> queueStr;
    string veriStr;
    // ��������Ϣ�������
    for (auto it : str) {
        queueStr.push(it);
        cout << it;
    }
    cout << "#�������̣�" << endl;
    cout << "��ʼ����#��ջ��" << startSymbol << "��ջ" << endl;
    queueStr.push("#");
    // ȡ����֤�ַ���
    veriStr = queueStr.front();
    queueStr.pop();

    stackStr.push("#");
    stackStr.push(startSymbol);
    while (!stackStr.empty()) {
        // ȡ��ջԪ��
        string trans = stackStr.top();
        stackStr.pop();
        if (trans == veriStr) {
            if (queueStr.empty()) {
                if (stackStr.empty()) {
                    cout << "��ջ X = " << setw(2) << left << trans << "�� ���� c = " << setw(2) << veriStr << "��ƥ�䣬�ɹ�;" << endl;
                    break;
                }
                else {
                    cout << "ƥ��ʧ��";
                    break;
                }
            }
            else {
                cout << "��ջ X = " << setw(2) << left << trans << "�� ���� c = " << setw(2) << veriStr << "��ƥ�䣬����ָ�����;" << endl;
                veriStr = queueStr.front();
                queueStr.pop();
                continue;
            }
        }
        auto it = table.find({ trans, veriStr });
        if (it != table.end()) {
            string productionStr;
            bool veri = true;
            productionStr += trans + "->";
            for (const auto& symbol : it->second) {
                productionStr += symbol;
            }
            for (auto it = table[{trans, veriStr}].rbegin(); it != table[{trans, veriStr}].rend(); ++it) {
                if (*it != "��") {
                    stackStr.push(*it);
                }
                else {
                    veri = false;
                }
            }
            if (veri) {
                cout << "��ջ X = " << setw(2) << left << trans << "�� ���� c = " << setw(2) << veriStr << "�����M[X, c] = " << productionStr << "������ʽ�Ҳ�������ջ��" << endl;
            }
            else {
                cout << "��ջ X = " << setw(2) << left << trans << "�� ���� c = " << setw(2) << veriStr << "�����M[X, c] = " << productionStr << endl;
            }

        }
        else {
            cout << "ƥ��ʧ��";
            break;
        }
    }
}

void Grammar::getFollow() {
    map<string, set<string>> followTempSet;  //���ڼ�¼first�ķ��ս��
    followSet[startSymbol].insert("#");
    for (auto left : nonTerminals) {
        for (auto vect : productions[left]) {
            int size = vect.size();
            for (int i = 0; i < size; i++) {
                if (nonTerminals.find(vect[i]) != nonTerminals.end()) { // ����Լ��Ƿ��ս��
                    for (int j = i + 1; j < size + 1; j++) {            // �����follow
                        bool veri = false;
                        if (j == size) {
                            followTempSet[vect[i]].insert(left);
                        }
                        else if (terminals.find(vect[j]) != terminals.end()) {  //���followΪ�ս��
                            followSet[vect[i]].insert(vect[j]);
                        }
                        else {  //���followΪ�ǽ��
                            for (auto it : firstSet[vect[j]])
                                if (it != "��")
                                    followSet[vect[i]].insert(it);
                                else
                                    veri = true;
                        }
                        if (!veri)
                            break;
                    }
                }
            }
        }
    }
    bool flag = true;
    while (flag) {
        flag = false;
        for (auto left : nonTerminals) {
            int size = followSet[left].size();
            for (auto temp : followTempSet[left]) {
                for (auto it : followSet[temp]) {
                    followSet[left].insert(it);
                }
            }
            if (size < followSet[left].size())
                flag = true;
        }
    }
}

//map<pair<string, vector<string>>, set<string>> selectSet
//map<string, set<vector<string>>> productions;
void Grammar::getSelect() {
    for (auto production : productions) {
        for (auto right : production.second) {
            for (auto str : right) {
                bool veri = false;
                if (str == "��") {
                    for (auto it : followSet[production.first])
                        selectSet[pair(production.first, right)].insert(it);
                    break;
                }
                if (terminals.find(str) != terminals.end()) {   // �ұ�Ϊ�ս��
                    selectSet[pair(production.first, right)].insert(str);
                }
                else {
                    for (auto it : firstSet[str]) {
                        if (it != "��")
                            selectSet[pair(production.first, right)].insert(it);
                        else
                            veri = true;
                    }
                }
                if (!veri)
                    break;
            }
        }
    }
}

// ����ķ�����ϸ��Ϣ
void Grammar::printGrammar() {
    cout << "CFG=(VN,VT,P,S)" << endl;

    // ������ս������ (VN)
    cout << "VN: ";
    for (auto temp : nonTerminals) {
        cout << temp << " ";
    }
    cout << endl;

    // ����ս������ (VT)
    cout << "VT: ";
    for (auto temp : terminals) {
        cout << temp << " ";
    }
    cout << endl;

    // �������ʽ���� (P)
    cout << "Productions: " << endl;
    int veri = 0;
    for (auto& production : productions) {
        for (auto& right : production.second) {
            cout << setw(2) << left << veri++ << ":"
                << setw(2) << production.first << "->";
            for (auto& it : right) {
                cout << it << " ";
            }
            cout << endl;
        }
    }
    cout << "StartSymbol: " << startSymbol << endl;

    // ��� First ����
    cout << "[First Set]" << endl;
    for (const auto& entry : firstSet) {
        cout << setw(2) << left << entry.first << ":";
        for (const auto& symbol : entry.second) {
            cout << symbol << " ";
        }
        cout << endl;
    }

    // ��� Follow ����
    cout << "[Follow Set]" << endl;
    for (const auto& entry : followSet) {
        cout << setw(2) << left << entry.first << ":";
        for (const auto& symbol : entry.second) {
            cout << symbol << " ";
        }
        cout << endl;
    }

    // ��� Select ����
    cout << "[Select Set]" << endl;
    int j = 0;
    for (const auto& entry : selectSet) {
        cout << setw(2) << left << j++ << ":"
            << setw(2) << entry.first.first << "-> ";
        for (auto it : entry.first.second) {
            cout << it << " ";
        }
        cout << ": ";
        for (const auto& symbol : entry.second) {
            cout << symbol << " ";
        }
        cout << endl;
    }
}

vector<string> read() {
    string line;
    cout << "��������Ҫ�����ķ��Ŵ��飺";

    // ��ȡ��������
    getline(cin, line);

    // ʹ�� stringstream ���зָ�
    stringstream ss(line);
    string word;
    vector<string> words;

    // ������ȡֱ��û�и���ĵ���
    while (ss >> word) {
        words.push_back(word);
    }

    return words;
}


int main(int argc, char* argv[]) {
    string fileName = (argc < 2) ? "D:\\study\\רҵ��\\����ԭ��\\Lab5\\test.txt" : argv[1];
    cout << "�����txtΪ��" << fileName << endl;
    Grammar grammar(fileName);
    grammar.getFirst();
    grammar.getFollow();
    grammar.getSelect();
    grammar.printGrammar();
    grammar.getTable();
    vector<string> str = read();
    grammar.analyseStr(str);
    return 0;
}