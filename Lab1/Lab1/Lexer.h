#pragma once
#ifndef LEXER_H_
#define LEXER_H_

#include<string>
#include<vector>

// Token类
// 一个Token类代表一个二元组，形式为（种别编码syn，字符串单词token或整型单词sum）
class Token
{
public:
    Token(int syn, const std::string& tokenStr, double sum);  // 种别编码， 字符串， 整型单词
    int getSyn();
    std::string getToken();
    double getSum();

private:
    int syn;    //种别编码
    std::string token;  //单词字符串
    double sum;    //数值单词（支持小数）
};

// 词法分析器
class Lexer
{
public:
    Lexer();    //构造函数
    std::string preprocess(const std::string& src); //预处理
    std::vector<Token> lexerAnalyse(const std::string& src);    //词法分析

private:
    bool isLetter(char ch); //是否为字母
    bool isDigit(char ch);  //是否为数字
};


#endif 