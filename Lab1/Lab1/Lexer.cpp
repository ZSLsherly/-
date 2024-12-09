#include"Lexer.h"
#include"Table.h"
#include<cctype>


using namespace std;

// ʵ��Token��
// ���캯��, ��ʼ���б�
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

// ʵ��Lexer��
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
    bool inComment = false; // ����ע��ģʽ
    bool inSingleLineComment = false;   // ����ע��

    for (int i = 0; i < src.size(); i++) {
        // �������ע��
        if (!inSingleLineComment && i < src.size() - 1 && src[i] == '/' && src[i + 1] == '*') {
            inComment = true;   // ����ע��
            i++;    // ����ע�ͱ��
        }
        // ������ע��
        else if (!inComment && i < src.size() - 1 && src[i] == '/' && src[i + 1] == '/') {
            inSingleLineComment = true; // ����ע��
            i++;    // ����ע�ͱ��
        }
        // ����ע��ģʽ����
        else if (inComment && i < src.size() - 1 && src[i] == '*' && src[i + 1] == '/') {
            inComment = false;
            i++;  // ����ע�ͱ��
        }
        // ����ע��ģʽ����
        else if (inSingleLineComment && src[i] == '\n') {
            inSingleLineComment = false;
        }
        // �������ע��ģʽ�У������ַ�
        else if (!inSingleLineComment && !inComment) {
            // �ǿո�
            if (!isspace(src[i])) {
                result += src[i]; // ����ַ�
            }
            // �����ֶ���հ��ַ�ʱ��ֻ����һ���ո񣬽�����հ��ַ���Ϊһ���ո�
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
    double result = 0;  // ���ڴ���С��
    std::string token;
    int syn = 0;

    for (int i = 0; i < src.size();)
    {
        // ����ĸΪ��ͷ�������ǵ���token��Ҳ�����ǹؼ���
        // ���»���Ϊ��ͷ��һ���ǵ���token
        if (isLetter(src[i]) || src[i] == '_')
        {
            token.clear();
            while (i < src.size() && (isLetter(src[i]) || isDigit(src[i]) || src[i] == '_'))
                token += src[i++];
            // �ؼ��֣�Ѱ�Ҷ�Ӧ���ֱ���
            if (keywordTable.find(token) != keywordTable.end())
            {
                syn = keywordTable[token];
            }
            // ����token���ֱ���Ϊ25
            else
            {
                syn = 25;
            }
            tokens.push_back({ syn, token, 0 });
        }
        // �������͵��ʣ�֧�ָ���
        else if (isDigit(src[i]) || (i < src.size() - 1 && src[i] == '-' && isDigit(src[i + 1])))
        {
            bool isNegative = (src[i] == '-');
            bool isDecimal = false;
            if (isNegative) i++; // ��������
            sum = 0; result = 0;    // ���³�ʼ��
            /*������������*/
            while (i < src.size() && isDigit(src[i]))
            {
                sum = sum * 10 + (src[i] - '0');
                i++;
            }
            result = sum;   // sumΪ�������֣��������ָ�ֵ��result
            /*����Ƕ���С���㵼�µ��˳�����˵���������С��������С������*/
            if (i < src.size() && src[i] == '.' && i < src.size() - 1 && isDigit(src[i + 1]))
            {
                isDecimal = true;
                i++;    // ����С����
                double decimalPlace = 0.1;  // ����С������
                /*����С��*/
                while (i < src.size() && isDigit(src[i]))
                {
                    result += (src[i] - '0') * decimalPlace;
                    decimalPlace *= 0.1;
                    i++;
                }
            }
            /*��������*/
            if (isNegative)
            {
                sum = -1 * sum;
                result = -1 * result;
            }
            tokens.push_back({ 26, "", result });
        }
        // �������
        else
        {
            token = src[i];
            syn = 0;
            // ���������ַ��ķ��ţ����Ⱥŵģ�>=, <=, !=, :=, +=, -=, *=, /=��
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
            // ���������ַ��ķ��ţ�++��--��
            else if (i < src.size() - 1 &&
                ((src[i] == '+' && src[i + 1] == '+') ||
                    (src[i] == '-' && src[i + 1] == '-')))
            {
                token += src[i + 1];
                syn = symbolTable[token];
                i += 2;
            }
            // ����������ţ�Ϊ���븺������
            else if (i < src.size() - 1 && isDigit(src[i - 1]) && src[i] == '-' && isDigit(src[i + 1]))
            {
                syn = symbolTable[token];
                i++;
            }
            // �����ַ��ķ���
            else if (symbolTable.find(token) != symbolTable.end())
            {
                syn = symbolTable[token];
                i++;
            }
            // δʶ���ַ�
            else
            {
                i++;
            }
            tokens.push_back({ syn, token, 0 });
        }
    }
    return tokens;
}
