#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "generate.h"
#include "parser.h"

void generateExpr(ASTNode * expr, FILE *f)
{
	/*if (expr->type == EXPRESSION) {
		fprintf(f, "movl  $%d,%%eax\n", expr->value);
	}
	else {
		generateExpr(expr->unaryExpression.expression, f);
		switch (expr->unaryExpression.unaryOp) {
		case '-':
			fprintf(f, "neg  %%eax\n");
			break;
		case '!':
			fprintf(f, "cmpl  $0, %%eax\n");
			fprintf(f, "movl  $0, %%eax\n");
			fprintf(f, "sete  %%al\n");
			break;
		case '~':
			fprintf(f, "not  %%eax\n");
			break;
		}
	}*/
}

void generateStatement(ASTNode * statement, FILE *f)
{
	generateExpr(statement->statement.expression, f);
	fprintf(f, "ret\n");
}

void generateFunction(ASTNode * function, FILE *f)
{
	fprintf(f, " .globl _%s\n", function->function.name);
	fprintf(f, "_%s:\n", function->function.name);
	generateStatement(function->function.body, f);
}

void generateAssembly(ASTNode * root, FILE *f)
{
	//generateFunction(root->program.children, f);
}
