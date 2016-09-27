%option noyywrap

%{
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stack>
#include "ast.h"
#include "tokens.h"

using namespace std;

int line = 0;
int dedents = 0;

stack<int> tab_stk;

%}

%x start_of_line

DIGIT [0-9]
LETTER [a-zA-Z]

%%
%{
		if(dedents>0){
			dedents--;
			return DEDENT;
		}
%}
<start_of_line>^" "+ {
			BEGIN(INITIAL);
			if (yyleng > tab_stk.top()) {
			    //cout << "start_of_line 1: INDENT " << tab_stk.top() << " " << yyleng << endl;
			    tab_stk.push(yyleng);
			    return INDENT;
			} else if (yyleng < tab_stk.top()) {
			    //cout << "start_of_line 1: DEDENT " << tab_stk.top() << endl;
			    tab_stk.pop();
					while(yyleng < tab_stk.top()){
						dedents++;
					  tab_stk.pop();
					}
			    return DEDENT;
			} else if (yyleng == tab_stk.top()) {
			}
		      }
<start_of_line>. {
		    //cout << "start_of_line 2: " << yytext << endl;
		    unput(yytext[0]);
		    BEGIN(INITIAL);
		    if (tab_stk.top() != 0) {
					tab_stk.pop();
					//cout << "start_of_line 2: DEDENT" << endl;
					while(tab_stk.top() != 0){
						dedents++;
						tab_stk.pop();
					}
			return DEDENT;
		    }
		 }
[ \t] /* Nada */
"<" { return '<'; }
">" { return '>'; }

">=" {return TK_MAYORIGUAL;}
"<=" {return TK_MENORIGUAL;}
"==" {return TK_IGUALDAD;}
"!=" {return TK_DESIGUALDAD;}

"=" { return '='; }
"+" { return '+'; }
"-" { return '-'; }
"*" { return '*'; }
"/" { return '/'; }
"(" { return '('; }
")" { return ')'; }
":" { return ':'; }
\n  { line++; BEGIN(start_of_line); return TK_NEWLINE; }
"print" { return KW_PRINT; }
"for" { return KW_FOR; }
"while" { return KW_WHILE; }
"def" { return KW_DEF; }
"main" { return KW_MAIN; }
"return" { return KW_RETURN; }
"true" { return KW_TRUE; }
"false" { return KW_FALSE; }
"not" { return KW_NOT; }
"if" { return KW_IF; }
"else" { return KW_ELSE; }
{DIGIT}+ { yylval.num_t = atoi(yytext); return TK_NUM; }
{LETTER}({DIGIT}|{LETTER})* { yylval.id_t = strdup(yytext); return TK_ID; }
"#"[^\n]* { line++; BEGIN(start_of_line);  return TK_NEWLINE; }
.   { printf("Simbolo no valido\n"); }

%%
