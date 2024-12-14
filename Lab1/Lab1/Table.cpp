#include "Table.h"
#include <iostream>
#include <iomanip>
#include <map>
#include <string>

using namespace std;

// 67为未知字符，68为数字
// 关键字表
std::map<std::string, int> keywordTable = {
    {"main", 1}, {"if", 2}, {"then", 3}, {"else", 4}, {"while", 5},
    {"do", 6}, {"repeat", 7}, {"until", 8}, {"for", 9}, {"from", 10},
    {"to", 11}, {"step", 12}, {"switch", 13}, {"case", 14}, {"default", 15},
    {"return", 16}, {"integer", 17}, {"real", 18}, {"char", 19}, {"bool", 20},
    {"and", 21}, {"or", 22}, {"not", 23}, {"mod", 24}, {"read", 25},
    {"write", 26}, {"static", 27}, {"int", 28}, {"double", 29}, {"struct", 30},
    {"break", 31}, {"long", 32}, {"typedef", 33}, {"const", 34}, {"float", 35},
    {"short", 36}, {"continue", 37}, {"void", 38}, {"sizeof", 39}
};

// 符号表
std::map<std::string, int> symbolTable = {
    {"+", 40}, {"-", 41}, {"*", 42}, {"/", 43}, {":", 44},
    {":=", 45}, {"<", 46}, {"!=", 47}, {"<=", 48}, {">", 49},
    {">=", 50}, {"=", 51}, {";", 52}, {"(", 53}, {")", 54}, {"#", 55},
    {"{", 56}, {"}", 57}, {"[", 58}, {"]", 59}, {"++", 60}, {",", 61},
    {"--", 62}, {"+=", 63}, {"*=", 64}, {"/=", 65}, {"-=", 66},{"\'", 69}
};

// 输出关键字表和符号表
void printTable()
{
    // 使用iomanip库来控制输出格式
    std::cout << std::left << std::setw(20) << "Keyword/Symbol"
        << std::setw(10) << "Type"
        << std::setw(10) << "syn" << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;

    // 输出关键字表（按顺序）
    for (const auto& entry : keywordTable)
    {
        std::cout << std::left << std::setw(20) << entry.first
            << std::setw(10) << "Keyword"
            << std::setw(10) << entry.second << std::endl;
    }

    // 输出符号表（按顺序）
    for (const auto& entry : symbolTable)
    {
        std::cout << std::left << std::setw(20) << entry.first
            << std::setw(10) << "Symbol"
            << std::setw(10) << entry.second << std::endl;
    }

    std::cout << std::endl;
}
