// proj2.cpp by Andrew Berg for COP4020 Fall 2016 Robert Van Engelen

#define ID  259
#define NUM 260
#define LET 256
#define IN  257
#define END 258

#include <iostream>
#include <stack>
#include <string>
#include <vector>

long long expr();
long long term();
long long term_tail(long long);
long long factor();
long long factor_tail(long long);
std::string id();
void test_parse();
void getnext();
void match(int token);
void getnext();
int lookahead();
bool isNumber(char);
void pop();
void push(std::vector<std::pair<std::string, long long> > val);
void pushtemp(std::string str, long long i);
void poptemp();
long long lookup(std::string);
long long num();
void gotosemi();