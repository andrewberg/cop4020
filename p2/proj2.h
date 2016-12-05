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

int expr();
int term();
int term_tail(int);
int factor();
int factor_tail(int);
std::string id();
void test_parse();
void getnext();
void match(int token);
void getnext();
int lookahead();
bool isNumber(char);
void pop();
void push(std::string, int);
int lookup(std::string);
int num();