%{
#include <cstdio>
#include <string>
#include "ast.h"
#include <map>

using namespace std;

int yylex();

extern int line;

void yyerror(const char *str)
{
    printf("Line %d: %s\n", line, str);
}

map<string, int> vars;

#define YYERROR_VERBOSE 1

%}

%union {
    char *id_t;
    int  num_t;

    Expression * expression_t;
	Statement * statement_t;
	StatementList  * statement_list_t;
}

%token<num_t> TK_NUM
%token<id_t> TK_ID
%token TK_NEWLINE INDENT DEDENT KW_PRINT KW_IF KW_ELSE KW_FOR KW_WHILE KW_DEF KW_MAIN KW_RETURN KW_TRUE KW_FALSE KW_NOT

%type <expression_t> expr
%type <statement_t> statement opt_else print if assign while for
%type <statement_list_t> statement_list

%nonassoc '>' '<'  TK_MAYORIGUAL TK_MENORIGUAL TK_IGUALDAD TK_DESIGUALDAD

%left '+' '-'
%left '*' '/'

%%

input:   statement_list {
	StatementList::iterator iter = $1->begin();
	while(iter != $1->end()){
		Statement* s = *iter;
		cout << s->GenerateCode() << endl;
		iter++;
	}
// 	for(auto s : *$1){
// 	  cout << s->GenerateCode() << endl;
// 	}
      delete $1;
}
;

statement_list: statement_list statement { $$ = $1; $$->push_back($2);}
	|statement { $$ = new StatementList(); $$->push_back($1); }
	;

statement: print { $$ =$1; }
    | assign { $$=$1; }
    | if { $$=$1; }
    | while { $$=$1; }
    | for { $$=$1; }
;

print: KW_PRINT '(' expr ')' new_line  {$$ = new PrintStatement($3);}
;

if: KW_IF expr ':' new_line INDENT statement_list DEDENT opt_else {
        BlockStatement *trueBlock = new BlockStatement($6);
        $$ = new IfStatement($2,trueBlock,$8);}
;


while : KW_WHILE expr ':' new_line statement_list DEDENT {
          BlockStatement *block = new BlockStatement($5);
           $$ = new WhileStatement($2,block); }
;


for : KW_FOR '(' expr ',' expr ')' ':' new_line statement_list DEDENT {
          BlockStatement *block = new BlockStatement($9);
          $$ = new ForStatement($3,$5,block); }
;



assign: TK_ID '=' expr new_line  {$$ = new AssignStatement($1,$3);}
;

opt_else: KW_ELSE ':' new_line INDENT statement_list DEDENT { $$ = new BlockStatement($5); }
        | { $$= new BlockStatement(new StatementList()); }
;

new_line:  new_line TK_NEWLINE
      | TK_NEWLINE
;

expr: expr '<' expr 			{ $$ = new LesserExpression($1,$3);}
	| expr '>' expr 			{ $$ = new GreaterExpression($1,$3);}
	| expr TK_MAYORIGUAL expr 	{ $$ = new GreaterOrEqualExpression($1,$3);}
	| expr TK_MENORIGUAL expr 	{ $$ = new LesserOrEqualExpression($1,$3);}
	| expr TK_IGUALDAD expr 	{ $$ = new EqualityExpression($1,$3);}
	| expr TK_DESIGUALDAD expr 	{ $$ = new InequalityExpression($1,$3);}
	| expr '+' expr 	{ $$ = new AddExpression($1,$3);}
	| expr '-' expr   { $$ = new SubExpression($1,$3);}
    | expr '*' expr   { $$ = new MultExpression($1,$3);}
	| expr '/' expr   { $$ = new DivExpression($1,$3);}
    | '(' expr ')'       { $$ = $2; }
    | TK_NUM        { $$ = new NumExpression($1);}
    | TK_ID 		{ string str($1) ; $$ = new IdExpression(str);}
    | KW_TRUE       { $$= new NumExpression(1);}
    | KW_FALSE      { $$= new NumExpression(0);}

%%
