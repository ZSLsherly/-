#include"Table.h"
#include<iostream>
#include<iomanip>

using namespace std;

std::map<std::string, int>keywordTable = {
    {"main", 1}, {"if", 2}, {"then", 3}, {"while", 4}, {"do", 5},
    {"static", 6}, {"int", 7}, {"double", 8}, {"struct", 9}, {"break", 10},
    {"else", 11}, {"long", 12}, {"switch", 13}, {"case", 14}, {"typedef", 15},
    {"char", 16}, {"return", 17}, {"const", 18}, {"float", 19}, {"short", 20},
    {"continue", 21}, {"for", 22}, {"void", 23}, {"default", 39}, {"sizeof", 24}
};

std::map<std::string, int>symbolTable = {
    {"+", 27}, {"-", 28}, {"*", 29}, {"/", 30}, {":", 31},
    {":=", 32}, {"<", 33}, {"!=", 34}, {"<=", 35}, {">", 36},
    {">=", 37}, {"=", 38}, {";", 41}, {"(", 42}, {")", 43}, {"#", 0},
    {"{", 44}, {"}", 45}, {"[", 46}, {"]", 47}, {"++", 48}, {",", 49}, 
    {"--", 50}, {"+=", 51},{"*=", 52}, {"/=", 53}, {"-=", 54}, 
};

// 输出关键字表和符号表
void printTable()
{
    // 使用iomanip库来控制输出
    std::cout << std::left << std::setw(20) << "Keyword/Symbol"
        << std::setw(10) << "Type"
        << std::setw(10) << "syn" << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;

    // 输出关键字表
    for (const auto& entry : keywordTable) 
    {
        std::cout << std::left << std::setw(20) << entry.first
            << std::setw(10) << "Keyword"
            << std::setw(10) << entry.second << std::endl;
    }

    // 输出符号表
    for (const auto& entry : symbolTable) 
    {
        std::cout << std::left << std::setw(20) << entry.first
            << std::setw(10) << "Symbol"
            << std::setw(10) << entry.second << std::endl;
    }

    std::cout << std::endl;
}