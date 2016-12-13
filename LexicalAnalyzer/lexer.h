/* Andrew Berg's COP4020 Lexical Analyzer for Robert Van Engelen Fall 2016 */

#include <stdio.h>

void emit_let();
void emit_var();
void emit_do();
void emit_done();
void emit_if();
void emit_else();
void emit_endif();
void emit_for();
void emit_to();
void emit_until();
void emit_loop();
void emit_true();
void emit_false();
void emit_varname();
void emit_number(float);
void emit_string();
void emit_op(const char *);
void emit_eof();
void print_br();
void set_br();
void print_nbsp();
void incDepth();
void decDepth();
void emit_display();
void emit_newline();
void emit_eq();