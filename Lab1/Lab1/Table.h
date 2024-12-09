#pragma once
#ifndef TABLE_H_
#define TABLE_H_

#include<map>
#include<string>


extern std::map<std::string, int>keywordTable;
extern std::map<std::string, int>symbolTable;

void printTable();

#endif