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
	if (term->type == BINARY_OP) {
		for (int i = 0; i < term->term.factorCount; i++) {
			generateFactor(term->term.rightFactor[i], f);
			//Do something with op
		}
	}
	generateFactor(term->term.factor, f);
}

void generateExpr(ASTNode * expr, FILE *f)
{
	if (expr->type == BINARY_OP) {
		for (int i = 0; i < expr->expression.termCount; i++) {
			generateTerm(expr->expression.rightTerm[i], f);
			//Do something with op
		}
	}
	generateTerm(expr->expression.term, f);
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
