#include"Lexer.h"
#include"Table.h"
#include<cctype>


using namespace std;

// 实现Token类
// 构造函数, 初始化列表
Token::Token(int syn, const std::string& tokenStr, double sum) : syn(syn), token(tokenStr), sum(sum) {}

int Token::getSyn()
{
    return syn;
}

std::string Token::getToken()
{
    return token;
}

double Token::getSum()
{
    return sum;
}

// 实现Lexer类
Lexer::Lexer() {}

bool Lexer::isDigit(char ch)
{
    return std::isdigit(ch);
}

bool Lexer::isLetter(char ch)
{
    return std::isalpha(ch);
}

std::string Lexer::preprocess(const std::string& src) {
    std::string result;
    bool inComment = false; // 多行注释模式
    bool inSingleLineComment = false;   // 单行注释

    for (int i = 0; i < src.size(); i++) {
        // 处理多行注释
        if (!inSingleLineComment && i < src.size() - 1 && src[i] == '/' && src[i + 1] == '*') {
            inComment = true;   // 多行注释
            i++;    // 跳过注释标记
        }
        // 处理单行注释
        else if (!inComment && i < src.size() - 1 && src[i] == '/' && src[i + 1] == '/') {
            inSingleLineComment = true; // 单行注释
            i++;    // 跳过注释标记
        }
        // 多行注释模式结束
        else if (inComment && i < src.size() - 1 && src[i] == '*' && src[i + 1] == '/') {
            inComment = false;
            i++;  // 跳过注释标记
        }
        // 单行注释模式结束
        else if (inSingleLineComment && src[i] == '\n') {
            inSingleLineComment = false;
        }
        // 如果不在注释模式中，则处理字符
        else if (!inSingleLineComment && !inComment) {
            // 非空格
            if (!isspace(src[i])) {
                result += src[i]; // 添加字符
            }
            // 当出现多个空白字符时，只保留一个空格，将多个空白字符视为一个空格
            else if (isspace(src[i]) && !result.empty() && result.back() != ' ' ) {
                result += ' ';
            }
        }
    }
    return result;
}
std::vector<Token> Lexer::lexerAnalyse(const std::string& src)
{
    std::vector<Token> tokens;
    int sum = 0;
    double result = 0;  // 用于处理小数
    std::string token;
    int syn = 0;

    for (int i = 0; i < src.size();)
    {
        // 以字母为开头，可能是单词token，也可能是关键字
        // 以下划线为开头，一定是单词token
        if (isLetter(src[i]) || src[i] == '_')
        {
            token.clear();
            while (i < src.size() && (isLetter(src[i]) || isDigit(src[i]) || src[i] == '_'))
                token += src[i++];
            // 关键字，寻找对应的种别码
            if (keywordTable.find(token) != keywordTable.end())
            {
                syn = keywordTable[token];
            }
            // 单词token，种别码为25
            else
            {
                syn = 25;
            }
            tokens.push_back({ syn, token, 0 });
        }
        // 处理整型单词，支持负数
        else if (isDigit(src[i]) || (i < src.size() - 1 && src[i] == '-' && isDigit(src[i + 1])))
        {
            bool isNegative = (src[i] == '-');
            bool isDecimal = false;
            if (isNegative) i++; // 跳过负号
            sum = 0; result = 0;    // 重新初始化
            /*处理整数部分*/
            while (i < src.size() && isDigit(src[i]))
            {
                sum = sum * 10 + (src[i] - '0');
                i++;
            }
            result = sum;   // sum为整数部分，整数部分赋值给result
            /*如果是读到小数点导致的退出，则说明这个数是小数，进行小数处理*/
            if (i < src.size() && src[i] == '.' && i < src.size() - 1 && isDigit(src[i + 1]))
            {
                isDecimal = true;
                i++;    // 跳过小数点
                double decimalPlace = 0.1;  // 处理小数部分
                /*处理小数*/
                while (i < src.size() && isDigit(src[i]))
                {
                    result += (src[i] - '0') * decimalPlace;
                    decimalPlace *= 0.1;
                    i++;
                }
            }
            /*正负处理*/
            if (isNegative)
            {
                sum = -1 * sum;
                result = -1 * result;
            }
            tokens.push_back({ 26, "", result });
        }
        // 处理符号
        else
        {
            token = src[i];
            syn = 0;
            // 处理两个字符的符号，带等号的（>=, <=, !=, :=, +=, -=, *=, /=）
            if (i < src.size() - 1 &&
                ((src[i] == ':' && src[i + 1] == '=') ||
                    (src[i] == '>' && src[i + 1] == '=') ||
                    (src[i] == '<' && src[i + 1] == '=') ||
                    (src[i] == '!' && src[i + 1] == '=') ||
                    (src[i] == '+' && src[i + 1] == '=') ||
                    (src[i] == '-' && src[i + 1] == '=') ||
                    (src[i] == '*' && src[i + 1] == '=') ||
                    (src[i] == '/' && src[i + 1] == '=')))
            {
                token += src[i + 1];
                syn = symbolTable[token];
                i += 2;
            }
            // 处理两个字符的符号（++、--）
            else if (i < src.size() - 1 &&
                ((src[i] == '+' && src[i + 1] == '+') ||
                    (src[i] == '-' && src[i + 1] == '-')))
            {
                token += src[i + 1];
                syn = symbolTable[token];
                i += 2;
            }
            // 单独处理减号，为了与负数区分
            else if (i < src.size() - 1 && isDigit(src[i - 1]) && src[i] == '-' && isDigit(src[i + 1]))
            {
                syn = symbolTable[token];
                i++;
            }
            // 单个字符的符号
            else if (symbolTable.find(token) != symbolTable.end())
            {
                syn = symbolTable[token];
                i++;
            }
            // 未识别字符
            else
            {
                i++;
            }
            tokens.push_back({ syn, token, 0 });
        }
    }
    return tokens;
}
