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
    set<string> nonTerminals;    // 非终结符集合
    set<string> terminals;       // 终结符集合
    map<string, set<vector<string>>> productions;  // 产生式集合，key是非终结符，value是产生式右部的集合
    string startSymbol;             // 起始符号
    map<string, set<string>> firstSet;
    map<string, set<string>> followSet;
    map<pair<string, vector<string>>, set<string>> selectSet;
    map<pair<string, string>, vector<string>> table;

    void readGrammar(const string& fileName);
    void printGrammar();
    void getFirst();
    void getFollow();
    void getSelect();
    // 构造函数，读取文法文件
    Grammar(const string& fileName) {
        readGrammar(fileName);
    }

};

// 读取文法描述文件
void Grammar::readGrammar(const string& fileName) {
    ifstream file(fileName);
    if (!file) {
        cerr << "Error opening file: " << fileName << std::endl;
        return;
    }
    int numNonTerminals, numTerminals, numProductions;

    // 读取非终结符
    file >> numNonTerminals;
    for (int i = 0; i < numNonTerminals; i++)
    {
        string temp;
        file >> temp;
        nonTerminals.insert(temp);
    }

    // 读取终结符
    file >> numTerminals;
    for (int i = 0; i < numTerminals; i++)
    {
        string temp;
        file >> temp;
        terminals.insert(temp);
    }

    // 读取产生式
    file >> numProductions;
    file.get(); // 读取剩余的换行符
    for (int i = 0; i < numProductions; i++)
    {
        string line;
        getline(file, line);    // 读取一整行，再做处理
        istringstream iss(line);    // 将其转化为输入字符流
        string left, arrow, symbol;
        vector<string>right;

        iss >> left >> arrow;   // 读取左部和箭头
        while (iss >> symbol)
        {
            if (symbol == "蔚") symbol = "ε";    // 单独处理一下编码集导致的ε变成蔚的问题
            right.push_back(symbol);
        }

        productions[left].insert(right);
    }

    // 读取开始符
    file >> startSymbol;
    file.close();
}

void Grammar::getFirst() {
    bool flag = true;
    //map<string, set<vector<string>>> productions;
    // set<string> nonTerminals;    // 非终结符集合
    //set<string> terminals;
    map<string, set<string>> firstTempSet;  //用于记录first的非终结符
    for (auto left : nonTerminals) {
        for (auto vect : productions[left]) {
            bool veri = false;
            for (auto now : vect) {
                if (terminals.find(now) != terminals.end() || now == "ε") {
                    firstSet[left].insert(now);
                }
                else {
                    for (auto it : productions[now]) {
                        if (it[0] == "ε") {
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
                    if (it != "ε") firstSet[left].insert(it);
                }
            }
            if (size < firstSet[left].size())
                flag = true;
        }
    }
}


void Grammar::getFollow() {
    map<string, set<string>> followTempSet;  //用于记录first的非终结符
    followSet[startSymbol].insert("#");
    for (auto left : nonTerminals) {
        for (auto vect : productions[left]) {
            int size = vect.size();
            for (int i = 0; i < size; i++) {
                if (nonTerminals.find(vect[i]) != nonTerminals.end()) { // 如果自己是非终结符
                    for (int j = i + 1; j < size + 1; j++) {            // 如果无follow
                        bool veri = false;
                        if (j == size) {
                            followTempSet[vect[i]].insert(left);
                        }
                        else if (terminals.find(vect[j]) != terminals.end()) {  //如果follow为终结符
                            followSet[vect[i]].insert(vect[j]);
                        }
                        else {  //如果follow为非结符
                            for (auto it : firstSet[vect[j]])
                                if (it != "ε")
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
    for (auto left : productions) {
        for (auto left_second : left.second) {
            for (auto str : left_second) {
                bool veri = false;
                if (str == "ε") {
                    for (auto it : followSet[left.first])
                        selectSet[pair(left.first, left_second)].insert(it);
                    break;
                }
                if (terminals.find(str) != terminals.end()) {   // 右边为终结符
                    selectSet[pair(left.first, left_second)].insert(str);
                }
                else {
                    for (auto it : firstSet[str]) {
                        if (it != "ε")
                            selectSet[pair(left.first, left_second)].insert(it);
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

// 输出文法的详细信息
void Grammar::printGrammar() {
    cout << "CFG=(VN,VT,P,S)" << endl;

    // 输出非终结符集合 (VN)
    cout << "VN: ";
    for (auto temp : nonTerminals) {
        cout << temp << " ";
    }
    cout << endl;

    // 输出终结符集合 (VT)
    cout << "VT: ";
    for (auto temp : terminals) {
        cout << temp << " ";
    }
    cout << endl;

    // 输出产生式集合 (P)
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

    // 输出 First 集合
    cout << "[First Set]" << endl;
    for (const auto& entry : firstSet) {
        cout << setw(2) << left << entry.first << ":";
        for (const auto& symbol : entry.second) {
            cout << symbol << " ";
        }
        cout << endl;
    }

    // 输出 Follow 集合
    cout << "[Follow Set]" << endl;
    for (const auto& entry : followSet) {
        cout << setw(2) << left << entry.first << ":";
        for (const auto& symbol : entry.second) {
            cout << symbol << " ";
        }
        cout << endl;
    }

    // 输出 Select 集合
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


int main(int argc, char* argv[]) {
    string fileName = (argc < 2) ? "D:\\study\\专业课\\编译原理\\Lab5\\test.txt" : argv[1];
    cout << "载入的txt为：" << fileName << endl;
    Grammar grammar(fileName);
    grammar.getFirst();
    grammar.getFollow();
    grammar.getSelect();
    grammar.printGrammar();

    return 0;
}
