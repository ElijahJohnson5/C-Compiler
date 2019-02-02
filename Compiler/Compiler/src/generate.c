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
		generateExpr(factor->factor.logicalOrExp, f);
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
	case '%':
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "xor  %%edx, %%edx\n");
		fprintf(f, "idivl  %%ecx\n");
		fprintf(f, "movl  %%edx, %%eax\n");
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

void generateBitwiseShiftExpr(ASTNode * bitwiseShiftExp, FILE * f)
{
	if (bitwiseShiftExp->type == BINARY_OP) {
		generateBitwiseShiftExpr(bitwiseShiftExp->bitwiseShiftExp.rightBitwiseShiftExp, f);
		fprintf(f, "push  %%eax\n");
	}
	generateAdditiveExpr(bitwiseShiftExp->bitwiseShiftExp.additiveExp, f);
	if (!strcmp(bitwiseShiftExp->bitwiseShiftExp.op, "<<")) {
		fprintf(f, "pop  %%ecx\n");
		//TODO figure out shift
		//fprintf(f, "shl  %%eax, %%ecx\n");
	}
	else if (!strcmp(bitwiseShiftExp->bitwiseShiftExp.op, ">>")) {
		fprintf(f, "pop  %%ecx\n");
		//TODO figure out shift
		//fprintf(f, "shr  %%eax, %%ecx\n");
	}
}

void generateRelationalExpr(ASTNode * relaExpr, FILE * f)
{
	if (relaExpr->type == BINARY_OP) {
		generateRelationalExpr(relaExpr->relationalExp.rightRelationalExp, f);
		fprintf(f, "push  %%eax\n");
	}
	generateBitwiseShiftExpr(relaExpr->relationalExp.bitwiseShiftExp, f);
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

void generateBitwiseAndExpr(ASTNode * bitwiseAndExp, FILE * f)
{
	if (bitwiseAndExp->type == BINARY_OP) {
		generateBitwiseAndExpr(bitwiseAndExp->bitwiseAndExp.rightBitwiseAndExp, f);
		fprintf(f, "push  %%eax\n");
	}
	generateEqualityExpr(bitwiseAndExp->bitwiseAndExp.equalityExp, f);
	switch (bitwiseAndExp->bitwiseAndExp.op)
	{
	case '&':
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "and  %%ecx, %%eax\n");
	default:
		break;
	}
}

void generateBitwiseXorExpr(ASTNode * bitwiseXorExp, FILE * f)
{
	if (bitwiseXorExp->type == BINARY_OP) {
		generateBitwiseXorExpr(bitwiseXorExp->bitwiseXorExp.rightBitwiseXorExp, f);
		fprintf(f, "push  %%eax\n");
	}
	generateBitwiseAndExpr(bitwiseXorExp->bitwiseXorExp.bitwiseAndExp, f);
	switch (bitwiseXorExp->bitwiseXorExp.op)
	{
	case '^':
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "xorl  %%ecx, %%eax\n");
	default:
		break;
	}
}

void generateBitwiseOrExpr(ASTNode * bitwiseOrExp, FILE * f)
{
	if (bitwiseOrExp->type == BINARY_OP) {
		generateBitwiseOrExpr(bitwiseOrExp->bitwiseOrExp.rightBitwiseOrExp, f);
		fprintf(f, "push  %%eax\n");
	}
	generateBitwiseXorExpr(bitwiseOrExp->bitwiseOrExp.bitwiseXorExp, f);
	switch (bitwiseOrExp->bitwiseOrExp.op)
	{
	case '|':
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "orl  %%ecx, %%eax\n");
	default:
		break;
	}
}

void generateLogicalAndExpr(ASTNode * logAndExpr, FILE * f)
{
	if (logAndExpr->type == BINARY_OP) {
		generateLogicalAndExpr(logAndExpr->logicalAndExp.rightLogicalAndExp, f);
		fprintf(f, "push  %%eax\n");
	}
	generateBitwiseOrExpr(logAndExpr->logicalAndExp.bitwiseOrExp, f);
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
		generateExpr(expr->logicalOrExp.rightExp, f);
		fprintf(f, "push  %%eax\n");
	}
	generateLogicalAndExpr(expr->logicalOrExp.logicalAndExp, f);
	if (!strcmp(expr->logicalOrExp.op, "||")) {
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
