 // proj2.cpp by Andrew Berg for COP4020 Fall 2016 Robert Van Engelen

#include "proj2.h"

int current = 0;
long long number;
std::string name; // max identifier name length is 79
std::string buffer;
std::vector<std::pair<std::string, long long>> vec;

int main()
{
  printf("%lld", expr()); // calls expr and prints this with long long
  return 0;
}

// the <expr> intrepreter
long long expr()
{
  long long result;
  if (lookahead() == LET) { // looks for a let statement
    std::string name1;
    long long value;
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
    long long value = term(); // calls term() given
    result = term_tail(value); // calls term_tail() with teh given value
  }
  
  return result;
}

// the <term> intrepreter
long long term()
{
  long long result;

  long long value = factor(); // makes a call to factor grab the value
  result = factor_tail(value); // makes a call to factor with this value

  return result;
}

// the <term_tail> intrepreter
long long term_tail(long long val)
{
  long long result;
  
  if (lookahead() == '+') { // looks for the + token
    match('+'); // matches up to the + token
    long long tt2 = val + term(); // takes the parameter val and adds term()
    result = term_tail(tt2); // takes the result of termtail2 and passes to tt
  } else if (lookahead() == '-') { // looks for the - token
    match('-'); // matches to the - token
    long long tt2 = val - term(); // reads in value from term and subtracts
    result = term_tail(tt2); // passes onto termtail to evaluate another token
  } else { // if it is number or something else
    result = val; // returns the result if did not find the + or -
  }

  return result;
}

// the <factor> intrepreter
long long factor()
{
  long long result;

  if (lookahead() == '(') { // looks for the ( token for an expression
    match('('); // matches up to the (
    result = expr();
    match(')'); // mathces up to the )
  } else if (lookahead() == '-') { // looks for a negation
    match('-'); // match the negative sign
    result = -1 * factor(); // multiplies by -1
  } else if (lookahead() == ID) { // variable name
    result = lookup(name); // go through the vector 
    getnext(); // grabs the next variable to look at
  } else {
    result = num(); // if all else false then it's going to be a number
  }

  return result; // return the final result for the
}

// the <factor_tail> function
long long factor_tail(long long val)
{
  long long result;

  if (lookahead() == '*') { // look for a multiplcation symbol
    match('*'); // match up to the multiplication
    long long ft2 = val * factor(); // evaluate with inputted val and factor
    result = factor_tail(ft2); // call the result with factor_tail
  } else if (lookahead() == '/') { // look for a division symbol
    match('/'); // match up to the division symbol
    long long ft2 = val / factor(); // evaluate with inputted val and factor
    result = factor_tail(ft2); // call the result with factor_tail
  } else {
    result = val;
  }

  return result;
}

// getnext() improved version with strings
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
    scanf("%lld", &number); // scanf the value to number
  } else {
    current = c; 
  }
}

void match(int token)
{
  if (lookahead() == token)
    getnext();
  else { // report syntax error and exit TODO
    printf("Syntax error.");
    exit(0); // stops program
  }
}

int lookahead()
{
   if (current == 0) // first time around
      getnext();
   return current;
}

std::string id() // grabs next and then returns the name
{
  getnext();
  return name;
}

long long num() // grabs next and then returns the number
{
  getnext();
  return number;
}

bool isNumber(char c) // helper function for the scanner
{
  return isdigit(c); 
}

void push(std::string str, long long i) // push helper function
{
  vec.push_back(std::pair<std::string, long long> (str, i));
}

void pop() // pop helper function
{
  vec.pop_back();
}

long long lookup(std::string val) // lookup an id value
{
  for (int i = vec.size(); i >= 0; --i) { // reads vec from back and then
    if (vec[i].first == val) { // returns value from back if exists
      return vec[i].second;
    }
  }
  return -1;
}

void test_parse() // tests the parser for correctness
{
    while (lookahead() != EOF) {
      switch (lookahead()) {
       case ID:
           break;
        case NUM:
           printf("NUM=%lld\n", number);
           break;
        default:
           printf("%c\n", lookahead());
    }
    getnext(); 
  }
}