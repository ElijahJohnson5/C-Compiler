#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "token.h"
#include "lexer.h"
#include "generate.h"

int main(int argc, char **argv) {
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
		TokenList* tokenList = tokenizeFile(readFileToString(f), &tokenCount);
		ASTNode *root = parseProgram(&tokenList);
		prettyPrintAST(root);
		generateAssembly(root, writeTo);
		fclose(writeTo);
		//char buf[512];
		//sprintf(buf, "cmd.exe /c \"gcc -m32 %s -o out\"", file);
		//system(buf);
	}
	system("PAUSE");
	return 0;
}