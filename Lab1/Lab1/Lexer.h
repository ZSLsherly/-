#pragma once
#ifndef LEXER_H_
#define LEXER_H_

#include<string>
#include<vector>

// Token��
// һ��Token�����һ����Ԫ�飬��ʽΪ���ֱ����syn���ַ�������token�����͵���sum��
class Token
{
public:
    Token(int syn, const std::string& tokenStr, double sum);  // �ֱ���룬 �ַ����� ���͵���
    int getSyn();
    std::string getToken();
    double getSum();

private:
    int syn;    //�ֱ����
    std::string token;  //�����ַ���
    double sum;    //��ֵ���ʣ�֧��С����
};

// �ʷ�������
class Lexer
{
public:
    Lexer();    //���캯��
    std::string preprocess(const std::string& src); //Ԥ����
    std::vector<Token> lexerAnalyse(const std::string& src);    //�ʷ�����

private:
    bool isLetter(char ch); //�Ƿ�Ϊ��ĸ
    bool isDigit(char ch);  //�Ƿ�Ϊ����
};


#endif 