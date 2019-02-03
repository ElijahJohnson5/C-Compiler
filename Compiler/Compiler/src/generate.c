#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "generate.h"
#include "parser.h"
#include "ast.h"

//TODO implement << >>

void generateFactor(ASTNode * factor, FILE * f)
{
	if (factor->type == CONSTANT_INT) {
		fprintf(f, "movl  $%d, %%eax\n", factor->value);
	}
	else if (factor->type == UNARY_OPERATOR) {
		generateFactor(factor->factor.factor, f);
		switch (factor->factor.unaryOp) {
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
		generatePrecedenceExpr(factor->factor.precedenceExpr, f);
	}
}

void generateOpAssembly(char * op, FILE * f)
{
	if (strcmp(op, "||") == 0) {
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "orl  %%ecx, %%eax\n");
		fprintf(f, "movl  $0, %%eax\n");
		fprintf(f, "setne  %%al\n");
	}
	else if (strcmp(op, "&&") == 0) {
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "cmpl  $0, %%ecx\n");
		fprintf(f, "setne  %%cl\n");
		fprintf(f, "cmpl  $0, %%eax\n");
		fprintf(f, "movl  $0, %%eax\n");
		fprintf(f, "setne  %%al\n");
		fprintf(f, "andb  %%cl, %%al\n");
	}
	else if (strcmp(op, "|") == 0) {
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "orl  %%ecx, %%eax\n");
	}
	else if (strcmp(op, "^") == 0) {
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "xorl  %%ecx, %%eax\n");
	}
	else if (strcmp(op, "&") == 0) {
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "and  %%ecx, %%eax\n");
	}
	else if (strcmp(op, "==") == 0) {
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "cmpl  %%eax, %%ecx\n");
		fprintf(f, "movl  $0, %%eax\n");
		fprintf(f, "sete  %%al\n");
	}
	else if (strcmp(op, "!=") == 0) {
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "cmpl  %%eax, %%ecx\n");
		fprintf(f, "movl  $0, %%eax\n");
		fprintf(f, "setne  %%al\n");
	}
	else if (strcmp(op, ">") == 0) {
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "cmpl  %%ecx, %%eax\n");
		fprintf(f, "movl  $0, %%eax\n");
		fprintf(f, "setg  %%al\n");
	}
	else if (strcmp(op, "<") == 0) {
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "cmpl  %%ecx, %%eax\n");
		fprintf(f, "movl  $0, %%eax\n");
		fprintf(f, "setl  %%al\n");
	}
	else if (strcmp(op, "<=") == 0) {
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "cmpl  %%ecx, %%eax\n");
		fprintf(f, "movl  $0, %%eax\n");
		fprintf(f, "setle  %%al\n");
	}
	else if (strcmp(op, ">=") == 0) {
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "cmpl  %%ecx, %%eax\n");
		fprintf(f, "movl  $0, %%eax\n");
		fprintf(f, "setge  %%al\n");
	}
	else if (strcmp(op, "<<") == 0) {
		fprintf(f, "pop  %%ecx\n");
	}
	else if (strcmp(op, ">>") == 0) {
		fprintf(f, "pop  %%ecx\n");
	}
	else if (strcmp(op, "+") == 0) {
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "addl  %%ecx, %%eax\n");
	}
	else if (strcmp(op, "-") == 0) {
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "subl  %%ecx, %%eax\n");
	}
	else if (strcmp(op, "*") == 0) {
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "imul  %%ecx, %%eax\n");
	}
	else if (strcmp(op, "/") == 0) {
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "xor  %%edx, %%edx\n");
		fprintf(f, "idivl  %%ecx\n");
	}
	else if (strcmp(op, "%") == 0) {
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "xor  %%edx, %%edx\n");
		fprintf(f, "idivl  %%ecx\n");
		fprintf(f, "movl  %%edx, %%eax\n");
	}
}

void generatePrecedenceExpr(ASTNode * precedenceExpr, FILE * f)
{
	if (precedenceExpr->type == BINARY_OP) {
		generatePrecedenceExpr(precedenceExpr->precedanceExp.rightExp, f);
		fprintf(f, "push  %%eax\n");
	}
	generateFactor(precedenceExpr->precedanceExp.exp, f);

	generateOpAssembly(precedenceExpr->precedanceExp.op, f);
}

void generateExpr(ASTNode * expr, FILE *f)
{
	if (expr->type == ASSIGN_EXPRESSION) {
		generateExpr(expr->expression.expression, f);
	}
	else {
		generatePrecedenceExpr(expr->expression.precedenceExp, f);
	}
	//Do stuff with op
}

void generateStatement(ASTNode * statement, FILE *f)
{
	generateExpr(statement->statement.expression, f);
}

void generateFunction(ASTNode * function, FILE *f)
{
	fprintf(f, " .globl _%s\n", function->function.name);
	fprintf(f, "_%s:\n", function->function.name);
	fprintf(f, "push  %%ebp\n");
	fprintf(f, "movl  %%esp, %%ebp\n");
	for (int i = 0; i < function->function.statementCount; i++) {
		generateStatement(function->function.body[i], f);
	}
	fprintf(f, "movl  %%ebp, %%esp\n");
	fprintf(f, "pop  %%ebp");
	fprintf(f, "ret  \n");
}

void generateAssembly(ASTNode * root, FILE *f)
{
	generateFunction(root->program.children, f);
}
