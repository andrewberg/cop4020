 // proj2.cpp by Andrew Berg for COP4020 Fall 2016 Robert Van Engelen

#include "proj2.h"

int current = 0;
int number;
std::string name; // max identifier name length is 79
std::string buffer;
std::vector<std::pair<std::string, int>> vec;

int main()
{
  int x = expr();

  std::cout << "Result: " << x << std::endl;
  //test_parse();
  return 0;
}

/* the <expr> parser */
int expr()
{
  int result;
  if (lookahead() == LET) {
    std::string name1;
    int value;
    match(LET);         // let
    name1 = id();       // id
    match('=');         // =
    value = expr();     // <expr>
    match(IN);          // in
    push(name1, value); // <push>
    result = expr();    // <expr>
    match(END);         // end
    pop();              // <pop>
  } else {
    int value = term(); 
    result = term_tail(value);
  }
  
  return result;
}

// the <term> intrepreter
int term()
{
  int result;

  int value = factor();
  ////std::cout << "Value in factor: " << value << std::endl;
  result = factor_tail(value);

  return result;
}

// the <term_tail> parser
// val = term.value
int term_tail(int val)
{
  int result;
  if (lookahead() == '+') {
    match('+');
    int tt2 = val + term();
    result = tt2;
  } else if (lookahead() == '-') {
    match('-');
    int tt2 = val - term();
    result = tt2;
  } else {
    result = val;
  }

  return result;
}

// the <factor> parser
int factor()
{
  ////std::cout << "In factor" << std::endl;
  int result;

  if (lookahead() == '(') { // +
    //std::cout << "New expr." << std::endl;
    match('(');
    result = expr();
    match(')');
  } else if (lookahead() == '-') { // - 
    //std::cout << "in negation" << std::endl;
    match('-');
    result = -1 * factor();
  } else if (lookahead() == ID) { // variable name
    result = lookup(name);
    getnext();
  } else {
    //getnext();
    result = num();
    //std::cout << "Result in factor num: " << result << std::endl;
  }

  return result;
}

int factor_tail(int val)
{
  int result;

  if (lookahead() == '*') { // +
    match('*');
    int ft2 = val * factor();
    result = ft2;
  } else if (lookahead() == '/') { // - 
    match('/');
    int factorval = factor();
    int ft2 = val / factorval;
    result = ft2;
  } else {
    result = val;
    ////std::cout << "Result in factor num: " << result << std::endl;
  }

  return result;
}

// getnext() improved version
void getnext()
{
  int c; 
  do
    c = getchar(); // read until no more spaces
  while (isspace(c));
   
  if (isalpha(c)) { // check if alphabetical character
    //iskeyword checks for 'L' and 'I' and 'N'
    name = ""; // reset name buffer to empty string
    ungetc(c, stdin); // unget c to stdin
    do { // do while loop
      c = getchar(); // grab c from stdin
      if (isalpha(c)) { // check isalpha
        name += c; // if isalpha then add to the name
      }
    } while (isalpha(c)); // until not an alphabetical character
    
    ungetc(c, stdin); // unget that last character that made do
                      // while fail
    
    /*
      * Go through processing whether a keyword or if it is a 
      * ID such as x and set current value accordingly
      * LET END and IN are #define at top of the file
     */

    if (name == "let") {
      current = LET;
    } else if (name == "in") {
      current = IN;
    } else if (name == "end") {
      current = END;
    } else {
      current = ID;
    }

  } else if (isNumber(c)) { // check if a number
    current = NUM; // set current to #define value of NUM
    ungetc(c, stdin); // unget that value that made isNumber true
    scanf("%d", &number); // scanf the value to number
  } else {
    current = c; 
  }
}

void match(int token)
{
   if (lookahead() == token)
      getnext();
   else
      ; // report syntax error and exit
}

int lookahead()
{
   if (current == 0) // first time around
      getnext();
   return current;
}

std::string id()
{
  getnext();
  //std::cout << "id() = " << name << std::endl;
  return name;
}

int num()
{
  getnext();
  return number;
}

bool isNumber(char c) // helper function for the scanner
{
  return isdigit(c); 
}

void push(std::string str, int i) // push helper function
{
  vec.push_back(std::pair<std::string, int> (str, i));
  //std::cout << "pushing back: " << str << " " << i << std::endl;
}

void pop() // pop helper function
{
  //std::cout << "popping " << vec[vec.size()-1].first << std::endl;
  vec.pop_back();
}

int lookup(std::string val) // lookup an id value
{
  for (int i = 0; i < vec.size(); ++i) {
    if (vec[i].first == val) {
      return vec[i].second;
    }
  }
  //std::cout << "failed to find " << val << " in the storage" << std::endl;
  return -1;
}

void test_parse()
{
    while (lookahead() != EOF) {
      switch (lookahead()) {
       case ID:
           //std::cout << "ID=" << name << std::endl;
           break;
        case NUM:
           printf("NUM=%d\n", number);
           break;
        default:
           printf("%c\n", lookahead());
    }
    getnext(); 
  }
}