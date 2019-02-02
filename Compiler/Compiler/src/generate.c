#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "generate.h"
#include "parser.h"

int eaxPushed = 0;

void generateFactor(ASTNode * factor, FILE * f)
{
	if (factor->type == CONSTANT_INT) {
		fprintf(f, "movl  $%d, %%eax\n", factor->value);
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
	else if (factor->type == EXPRESSION) {
		generateExpr(factor->factor.expression, f);
	}
}

void generateTerm(ASTNode * term, FILE * f)
{
	if (term->type == BINARY_OP) {
		generateTerm(term->term.rightTerm, f);
		fprintf(f, "push  %%eax\n");
	}
	generateFactor(term->term.factor, f);

	switch (term->term.op) {
	case '*':
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "imul  %%ecx, %%eax\n");
		break;
	case '/':
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "xor  %%edx, %%edx\n");
		fprintf(f, "idivl  %%ecx\n");
		break;
	}
}

void generateExpr(ASTNode * expr, FILE *f)
{
	if (expr->type == BINARY_OP) {
		generateExpr(expr->additiveExp.rightAdditiveExp, f);
		fprintf(f, "push  %%eax\n");
	}
	generateTerm(expr->additiveExp.term, f);
	
	switch (expr->additiveExp.op) {
	case '+':
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "addl  %%ecx, %%eax\n");
		break;
	case '-':
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "subl  %%eax, %%ecx\n");
		fprintf(f, "movl  %%ecx, %%eax\n");
	}
}

void generateStatement(ASTNode * statement, FILE *f)
{
	generateExpr(statement->statement.expression, f);
	fprintf(f, "ret  \n");
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
