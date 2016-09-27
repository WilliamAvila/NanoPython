#include <cstdio>
#include <stack>
#include "ast.h"
#include "tokens.h"

using namespace std;

extern stack<int> tab_stk;
extern int line;

int main()
{
    line = 1;
    tab_stk.push(0);
    yyparse();
}
