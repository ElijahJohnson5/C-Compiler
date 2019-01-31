#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "generate.h"
#include "parser.h"

char * generateReturnExpr(ASTNode * statement)
{
	char *buffer = calloc(50, sizeof(char));
	sprintf(buffer, "movl    $%d, %%eax\nret\n", statement->returnStatement.expression->value);
	return buffer;
}

char * generateFunction(ASTNode * function)
{
	char *functionBody = generateReturnExpr(function->function.body);
	char *functionText = malloc(strlen(".globl _") + strlen(function->function.name) + strlen(function->function.name) + 3 + strlen(functionBody));
	sprintf(functionText, ".globl _%s\n_%s:\n%s", function->function.name, function->function.name, functionBody);

	return functionText;
}

char * generateAssembly(ASTNode * root)
{
	return generateFunction(root->program.children);
}
