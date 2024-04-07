#include "node.h"
#include "syntax.tab.h"
#include <stdio.h>
extern ptrNode root;

extern int yylineno;
extern int yyparse();
extern void yyrestart(FILE *);

unsigned lexError = FALSE;
unsigned synError = FALSE;

int main(int argc, char **argv) {
    if (argc <= 1) {
        yyparse();
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        perror(argv[1]);
        return 1;
    }

    yyrestart(f);
    // yydebug = 1;
    yyparse();
    if (!lexError && !synError) {
        printTreeInfo(root, 0);
    }
    // printTreeInfo(root, 0);
    deleteNode(root);
    return 0;
}
