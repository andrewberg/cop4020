/* Andrew Berg's COP4020 Lexical Analyzer for Robert Van Engelen Fall 2016 */

%{
	#include "lexer.h"
	
	float number;
	const char *string;

	int isDec = 0;
	int needBr = 0;
  int needNl = 0;
  int noSpace = 0;
%}

isLoop (LOOP)
int [0-9]+
flt [-+][0-9]*([0-9]|\.[0-9]|[0-9]\.)[0-9]*([Ee][-+]?[0-9]+)?[FfLl]?
let (LET)
for (FOR)
equals (=)
operators [%&*+-/<>^|~]|={2,}
exc (!)
to TO

var (VAR)
until UNTIL
line [\n \t]+
do DO
done DONE
true (TRUE)
false (FALSE)
varName [$][^ \n\t\r]+
ifstate IF
else ELSE
endif ENDIF
display display
newline newline
isString \".*\"
eq "eq?"

%%
{isLoop} needNl = 1; decDepth(); needBr = 1; return 11;
{let} needNl = 1; needBr = 1; number = 0; return 1;
{var} needNl = 1; incDepth(); number = 0; isDec = 1; return 2;
{do}  needNl = 1; if (isDec) {decDepth(); isDec = 0; needBr = 1; } return 3;
{done} needNl = 1; decDepth(); needBr = 1; return 4;
{for} needNl = 1; needBr = 1; return 8;
{to}  return 9;
{equals} needNl = 1; string = yytext; return 14; 
{exc} string = yytext; return 15; 
{int}	number = (float)strtol(yytext, NULL, 10); return 17;
{flt}	number = strtod(yytext, NULL); return 17;
{line} return 50;
{true} return 12;
{false} return 13;
{varName} string = yytext; return 16;
{ifstate} needNl = 1; needBr = 1; return 5;
{until} needNl = 1; needBr = 1; return 10;
{else} needNl = 1; needBr = 1; decDepth(); return 6;
{endif} needNl = 1; needBr = 1; decDepth(); return 7;
{display} needBr = 1; return 20;
{operators} string = yytext; return 19;
{isString} string = yytext; return 18;
{newline} return 21;
{eq} return 22;
%%

int curDepth = 0;
int needSpace = 0;
int wasFor = 0;
int lastWasLoop = 0;
int lastToken = 0;
int notFirst = 0;
int isEnd = 0;
int oToken = 0;

int main(int argc, char **argv) {
	yyin = fopen(argv[1], "r");

	printf("<html>\n<body>");

	while (1) {
  		int token = yylex(); // get next token (or use Lexer.lex() in C++)
      //printf("%d", token);
      if (token == 0) {
        if (wasFor && lastWasLoop) printf("<br>");
        break;
      }
      
      lastWasLoop = 0;

      if (token == 2) { // handles var blocks
        emit_var();
        printf("<br>&nbsp;&nbsp;");

        while (token != 3) { // loops until 3
          token = yylex();

          if (oToken == 50 && token != 3 && notFirst > 1) { // if not first then " "
            printf(" ");
          } else {
            ++notFirst;
          }
          
          switch (token) {
            case 14: emit_op(string); break;
            case 17: emit_number(number); break;
            case 16: emit_string(string); break;
            case 18: emit_string(string); break;
          }
          oToken = token;
        }
        notFirst = 0;
      }


      if (needBr) {
        printf("\n<br>");
        needBr = 0;
        needSpace = 1;
      } else if (lastToken == 50) {
        printf(" ");
      }

    	switch (token) {
      	case 1: print_nbsp(); emit_let();  break;
      	case 2: emit_var(); break;
      	case 3: print_nbsp(); emit_do(); incDepth(); break;
      	case 4: print_nbsp(); emit_done(); break;
      	case 5: print_nbsp(); emit_if(); incDepth(); break;
      	case 6: print_nbsp(); emit_else(); incDepth(); break;
      	case 7: print_nbsp(); emit_endif(); break;
      	case 8: print_nbsp(); emit_for(); wasFor = 1; incDepth(); break;
      	case 9: emit_to(); break;
      	case 10: print_nbsp(); emit_until(); incDepth(); wasFor = 0; break;
      	case 11: print_nbsp();  emit_loop(); lastWasLoop = 1; break;
      	case 12: emit_true(); break;
      	case 13: emit_false(); break;
    		case 14: emit_op(string); break;
    		case 15: emit_op(string); break;
    		case 16: emit_string(string); break;
    		case 17: emit_number(number); break; // or use Lexer.number in C++
        case 18: emit_string(string); break;
        case 19: emit_string(string); break;
        case 20: print_nbsp(); emit_display(); break;
        case 21: emit_newline(); break;
        case 22: emit_eq(); break;
    		case 50: break;
        default: break;
		}

    lastToken = token;
	 }
 
 	printf("\n</body>\n</html>");
	fclose(yyin);
}

/* print methods for everything */

void incDepth() {
	++curDepth;
}

void decDepth() {
	--curDepth;
}

void emit_number(float number) {
  printf("<i>%g</i>", number);
}

void emit_for() {
	printf("<b>FOR</b>");
}

void emit_let() {
	printf("<b>LET</b>");
}

void emit_op(const char* op) {
	printf("<b>%s</b>", op);
}

void emit_loop() {
	printf("<b>LOOP</b>");
}

void emit_to() {
	printf("<b>TO</b>");
}

void emit_until() {
	printf("<b>UNTIL</b>");
}

void emit_eq() {
  printf("eq?");
}

void emit_var() {
	printf("\n<br><b>VAR</b>\n");
}

void emit_do() {
	printf("<b>DO</b>");
}

void emit_done() {
  printf("<b>DONE</b>");
}

void emit_else() {
  printf("<b>ELSE</b>");
}

void emit_false() {
  printf("<b>FALSE</b>");
}

void emit_if() {
  printf("<b>IF</b>");
}

void emit_string(const char* str) {
  printf("%s", str);
}

void emit_true() {
  printf("<b>TRUE</b>");
}

void emit_display() {
  printf("display");
}

void emit_endif() {
  printf("<b>ENDIF</b>");
}

void emit_newline() {
  printf("newline");
}

void print_nbsp() {
	for (int i = 0; i < curDepth; ++i) {
		printf("&nbsp;&nbsp;");
	}
}