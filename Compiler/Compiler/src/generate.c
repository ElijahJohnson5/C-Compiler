#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "generate.h"
#include "parser.h"

int onStack = 0;

void generateFactor(ASTNode * factor, FILE * f)
{
	if (factor->type == CONSTANT_INT) {
		fprintf(f, "movl  $%d,%%eax\n", factor->value);
		if (onStack == 0) {
			fprintf(f, "push  %%eax\n");
			onStack++;
		}
	}
	else if (factor->type == UNARY_OPERATOR) {
		generateFactor(factor->factor.factor.factor, f);
		switch (factor->factor.factor.unaryOp) {
		case '-':
			fprintf(f, "neg  %%eax\n");
			break;
		case '~':
			fprintf(f, "not  %%eax\n");
			break;
		case '!':
			fprintf(f, "cmpl  $0, %%eax\n");
			fprintf(f, "movl  $0, %%eax\n");
			fprintf(f, "sete  %%al\n");
			break;
		}
	}
	else {

	}
}

void generateTerm(ASTNode * term, FILE * f)
{
	for (int i = 0; i < term->term.factorCount; i++) {
		generateFactor(term->term.factor[i], f);
		if (term->term.op != NULL && i - 1 >= 0) {
			switch (term->term.op[i - 1]) {
			case '*':
				break;
			case '/':
				break;
			}
		}
	}
}

void generateExpr(ASTNode * expr, FILE *f)
{
	for (int i = 0; i < expr->expression.termCount; i++) {
		generateTerm(expr->expression.term[i], f);
		if (expr->term.op != NULL && i - 1 >= 0) {
			switch (expr->expression.op[i - 1]) {
			case '+':
				fprintf(f, "pop  %%ecx\n");
				fprintf(f, "addl  %%ecx, %%eax\n");
				break;
			case '-':
				break;
			}
			continue;
		}
	}
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
	generateFunction(root->program.children, f);
}
