#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "token.h"
#include "lexer.h"
#include "generate.h"
#include "ast.h"

int main(int argc, char **argv) {
	//TODO fix this function, look very bad
	if (argc > 1) {
		int tokenCount;
		FILE *writeTo;
		char *file = malloc(strlen(argv[1]) + 1);
		strncpy(file, argv[1], strlen(argv[1]) - 2);
		file[strlen(argv[1]) - 2] = '.';
		file[strlen(argv[1]) - 1] = 's';
		file[strlen(argv[1])] = '\0';
		FILE *f = fopen(argv[1], "r");
		writeTo = fopen(file, "w");
		char *s = readFileToString(f);
		fclose(f);
		TokenList* tokenList = tokenizeFile(s, &tokenCount);
		TokenList * headRef = tokenList;
		free(s);
		printTokenList(tokenList);
		ASTNode *root = parseProgram(&tokenList);
		freeTokenList(headRef);
		prettyPrintAST(root);
		//generateAssembly(root, writeTo);
		//fclose(writeTo);
		//freeAST(root);
		free(file);
	}
	system("PAUSE");
	return 0;
}