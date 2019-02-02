#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "generate.h"
#include "parser.h"
#include "ast.h"

int eaxPushed = 0;

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

void generateAdditiveExpr(ASTNode * addExpr, FILE * f)
{
	if (addExpr->type == BINARY_OP) {
		generateAdditiveExpr(addExpr->additiveExp.rightAdditiveExp, f);
		fprintf(f, "push  %%eax\n");
	}
	generateTerm(addExpr->additiveExp.term, f);
	//plus and minus
	switch (addExpr->additiveExp.op) {
	case '+':
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "addl  %%ecx, %%eax\n");
		break;
	case '-':
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "subl  %%eax, %%ecx\n");
		fprintf(f, "movl  %%ecx, %%eax\n");
		break;
	}
}

void generateRelationalExpr(ASTNode * relaExpr, FILE * f)
{
	if (relaExpr->type == BINARY_OP) {
		generateRelationalExpr(relaExpr->relationalExp.rightRelationalExp, f);
		fprintf(f, "push  %%eax\n");
	}
	generateAdditiveExpr(relaExpr->relationalExp.additiveExp, f);
	//Do stuff with op
	if (!strcmp(relaExpr->relationalExp.op, "<=")) {
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "cmpl  %%ecx, %%eax\n");
		fprintf(f, "movl  $0, %%eax\n");
		fprintf(f, "setle  %%al\n");
	}
	else if (!strcmp(relaExpr->relationalExp.op, ">=")) {
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "cmpl  %%ecx, %%eax\n");
		fprintf(f, "movl  $0, %%eax\n");
		fprintf(f, "setge  %%al\n");
	}
	else if (!strcmp(relaExpr->relationalExp.op, "<")) {
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "cmpl  %%ecx, %%eax\n");
		fprintf(f, "movl  $0, %%eax\n");
		fprintf(f, "setl  %%al\n");
	}
	else if (!strcmp(relaExpr->relationalExp.op, ">")) {
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "cmpl  %%ecx, %%eax\n");
		fprintf(f, "movl  $0, %%eax\n");
		fprintf(f, "setg  %%al\n");
	}
}

void generateEqualityExpr(ASTNode * eqExpr, FILE * f)
{
	if (eqExpr->type == BINARY_OP) {
		generateEqualityExpr(eqExpr->equalityExp.rightEqualityExp, f);
		//Push term onto stack
		fprintf(f, "push  %%eax\n");
	}
	generateRelationalExpr(eqExpr->equalityExp.relationalExp, f);
	if (!strcmp(eqExpr->equalityExp.op, "==")) {
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "cmpl  %%eax, %%ecx\n");
		fprintf(f, "movl  $0, %%eax\n");
		fprintf(f, "sete  %%al\n");
	} 
	else if (!strcmp(eqExpr->equalityExp.op, "!=")) {
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "cmpl  %%eax, %%ecx\n");
		fprintf(f, "movl  $0, %%eax\n");
		fprintf(f, "setne  %%al\n");
	}
}

void generateLogicalAndExpr(ASTNode * logAndExpr, FILE * f)
{
	if (logAndExpr->type == BINARY_OP) {
		generateLogicalAndExpr(logAndExpr->logicalAndExp.rightLogicalAndExp, f);
		fprintf(f, "push  %%eax\n");
	}
	generateEqualityExpr(logAndExpr->logicalAndExp.equalityExp, f);
	if (!strcmp(logAndExpr->logicalAndExp.op, "&&")) {
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "cmpl  $0, %%ecx\n");
		fprintf(f, "setne  %%cl\n");
		fprintf(f, "cmpl  $0, %%eax\n");
		fprintf(f, "movl  $0, %%eax\n");
		fprintf(f, "setne  %%al\n");
		fprintf(f, "andb  %%cl, %%al\n");
	}
	//Do stuff with op
}

void generateExpr(ASTNode * expr, FILE *f)
{
	if (expr->type == BINARY_OP) {
		generateExpr(expr->expression.rightExp, f);
		fprintf(f, "push  %%eax\n");
	}
	generateLogicalAndExpr(expr->expression.logicalAndExp, f);
	if (!strcmp(expr->expression.op, "||")) {
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "orl  %%ecx, %%eax\n");
		fprintf(f, "movl  $0, %%eax\n");
		fprintf(f, "setne  %%al\n");
	}
	//Do stuff with op
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
