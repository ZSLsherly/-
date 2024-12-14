#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include "Lexer.h"
#include "Table.h"

using namespace std;

int main(int argc, char* argv[])
{
   string filePath = (argc < 2) ? "../test.txt" : argv[1];

    // ���ı��ļ�
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << argv[1] << std::endl;
        return 1;
    }
    cout << filePath << endl;
    // ��ȡ�ļ�����
    std::string input((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    Lexer lexer;
    std::string processedInput = lexer.preprocess(input);
    std::vector<Token> tokens = lexer.lexerAnalyse(processedInput);

    // ����ʷ��������
    printTable();

    if (processedInput.size() == 0)
    {
        std::cout << "" << std::endl;
        return 0;
    }

    for (Token token : tokens) 
    {
        if (token.getSyn() == 67) 
        { // ��ʶ��
            std::cout << "syn: " << token.getSyn() << " Token: " << token.getToken() << std::endl;
        }
        else if (keywordTable.find(token.getToken()) != keywordTable.end()) 
        { // �ؼ���
            std::cout << "syn: " << token.getSyn() << " Keyword: " << token.getToken() << std::endl;
        }
        else if (symbolTable.find(token.getToken()) != symbolTable.end()) 
        { // ����
            std::cout << "syn: " << token.getSyn() << " Symbol: " << token.getToken() << std::endl;
        }
        else if (token.getSyn() == 68) 
        { // ����
            std::cout << "syn: " << token.getSyn() << " Digit: " << std::setprecision(2)<<token.getSum() << std::endl;
        }
        else 
        { // δ�������
            // �ո���δ�������
            if (!std::isspace(token.getToken()[0]))
                std::cout << "syn: -1 ERROR: Undefined Symbol : [" << token.getToken() << "]" << std::endl;
        }
    }

    return 0;
}
