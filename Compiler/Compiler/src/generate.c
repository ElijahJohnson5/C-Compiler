#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "generate.h"
#include "parser.h"
#include "ast.h"
#include "map.h"

//TODO implement << >>

int stackOffset = -4;

void generateFactor(ASTNode * factor, FILE * f, HashMap *map)
{
	if (factor->type == CONSTANT_INT) {
		fprintf(f, "movl  $%d, %%eax\n", factor->value);
	}
	else if (factor->type == UNARY_OPERATOR) {
		generateFactor(factor->factor.factor, f, map);
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
		generateLogicalOrExpr(factor->factor.expr, f, map);
	}
	else if (factor->type == VARIABLE) {
		fprintf(f, "movl  %d(%%ebp), %%eax\n", lookupNode(map, factor->factor.id));
	}
}


void generateTerm(ASTNode * term, FILE * f, HashMap *map)
{
	if (term->type == BINARY_OP) {
		generateTerm(term->term.rightTerm, f, map);
		fprintf(f, "push  %%eax\n");
		
	}
	generateFactor(term->term.factor, f, map);

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

void generateAdditiveExpr(ASTNode * addExpr, FILE * f, HashMap *map)
{
	if (addExpr->type == BINARY_OP) {
		generateAdditiveExpr(addExpr->additiveExp.rightAdditiveExp, f, map);
		fprintf(f, "push  %%eax\n");
		
	}
	generateTerm(addExpr->additiveExp.term, f, map);
	//plus and minus
	switch (addExpr->additiveExp.op) {
	case '+':
		fprintf(f, "pop  %%ecx\n");
		
		fprintf(f, "addl  %%ecx, %%eax\n");
		break;
	case '-':
		fprintf(f, "pop  %%ecx\n");
		
		fprintf(f, "subl  %%ecx, %%eax\n");
		//fprintf(f, "movl  %%ecx, %%eax\n");
		break;
	}
}

void generateBitwiseShiftExpr(ASTNode * bitwiseShiftExp, FILE * f, HashMap *map)
{
	if (bitwiseShiftExp->type == BINARY_OP) {
		generateBitwiseShiftExpr(bitwiseShiftExp->bitwiseShiftExp.rightBitwiseShiftExp, f, map);
		fprintf(f, "push  %%eax\n");
		
	}
	generateAdditiveExpr(bitwiseShiftExp->bitwiseShiftExp.additiveExp, f, map);
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

void generateRelationalExpr(ASTNode * relaExpr, FILE * f, HashMap *map)
{
	if (relaExpr->type == BINARY_OP) {
		generateRelationalExpr(relaExpr->relationalExp.rightRelationalExp, f, map);
		fprintf(f, "push  %%eax\n");
		
	}
	generateBitwiseShiftExpr(relaExpr->relationalExp.bitwiseShiftExp, f, map);
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

void generateEqualityExpr(ASTNode * eqExpr, FILE * f, HashMap *map)
{
	if (eqExpr->type == BINARY_OP) {
		generateEqualityExpr(eqExpr->equalityExp.rightEqualityExp, f, map);
		//Push term onto stack
		fprintf(f, "push  %%eax\n");
		
	}
	generateRelationalExpr(eqExpr->equalityExp.relationalExp, f, map);
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

void generateBitwiseAndExpr(ASTNode * bitwiseAndExp, FILE * f, HashMap *map)
{
	if (bitwiseAndExp->type == BINARY_OP) {
		generateBitwiseAndExpr(bitwiseAndExp->bitwiseAndExp.rightBitwiseAndExp, f, map);
		fprintf(f, "push  %%eax\n");
		
	}
	generateEqualityExpr(bitwiseAndExp->bitwiseAndExp.equalityExp, f, map);
	switch (bitwiseAndExp->bitwiseAndExp.op)
	{
	case '&':
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "and  %%ecx, %%eax\n");
		
	default:
		break;
	}
}

void generateBitwiseXorExpr(ASTNode * bitwiseXorExp, FILE * f, HashMap *map)
{
	if (bitwiseXorExp->type == BINARY_OP) {
		generateBitwiseXorExpr(bitwiseXorExp->bitwiseXorExp.rightBitwiseXorExp, f, map);
		fprintf(f, "push  %%eax\n");
		
	}
	generateBitwiseAndExpr(bitwiseXorExp->bitwiseXorExp.bitwiseAndExp, f, map);
	switch (bitwiseXorExp->bitwiseXorExp.op)
	{
	case '^':
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "xorl  %%ecx, %%eax\n");
		
	default:
		break;
	}
}

void generateBitwiseOrExpr(ASTNode * bitwiseOrExp, FILE * f, HashMap *map)
{
	if (bitwiseOrExp->type == BINARY_OP) {
		generateBitwiseOrExpr(bitwiseOrExp->bitwiseOrExp.rightBitwiseOrExp, f, map);
		fprintf(f, "push  %%eax\n");
		
	}
	generateBitwiseXorExpr(bitwiseOrExp->bitwiseOrExp.bitwiseXorExp, f, map);
	switch (bitwiseOrExp->bitwiseOrExp.op)
	{
	case '|':
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "orl  %%ecx, %%eax\n");
		
	default:
		break;
	}
}

void generateLogicalAndExpr(ASTNode * logAndExpr, FILE * f, HashMap *map)
{
	if (logAndExpr->type == BINARY_OP) {
		generateLogicalAndExpr(logAndExpr->logicalAndExp.rightLogicalAndExp, f, map);
		fprintf(f, "push  %%eax\n");
		
	}
	generateBitwiseOrExpr(logAndExpr->logicalAndExp.bitwiseOrExp, f, map);
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

void generateLogicalOrExpr(ASTNode * expr, FILE *f, HashMap *map)
{
	if (expr->type == BINARY_OP) {
		generateExpr(expr->logicalOrExp.rightExp, f, map);
		fprintf(f, "push  %%eax\n");
		
	}
	generateLogicalAndExpr(expr->logicalOrExp.logicalAndExp, f, map);
	if (!strcmp(expr->logicalOrExp.op, "||")) {
		fprintf(f, "pop  %%ecx\n");
		fprintf(f, "orl  %%ecx, %%eax\n");
		fprintf(f, "movl  $0, %%eax\n");
		fprintf(f, "setne  %%al\n");
		
	}
	//Do stuff with op
}

void generateExpr(ASTNode * expr, FILE *f, HashMap *map)
{
	if (expr->type == ASSIGN_EXPRESSION) {
		generateExpr(expr->expression.expression, f, map);
		fprintf(f, "movl  %%eax, %d(%%ebp)\n", lookupNode(map, expr->expression.id));
	}
	else if (expr->type == COMPOUND_ASSIGN_EXPRESSION) {
		generateExpr(expr->expression.expression, f, map);
	}
	else {
		generateLogicalOrExpr(expr->expression.precedenceExp, f, map);
	}
}

void generateStatement(ASTNode * statement, FILE *f, HashMap *map)
{
	if (statement->type == DECLARE_STATEMENT) {
		if (lookupNode(map, statement->statement.id) != -1) {
			printf("Variable already declared\n");
			return;
		}
		else {
			if (statement->statement.optinalAssignExpression != NULL) {
				generateExpr(statement->statement.optinalAssignExpression, f, map);
			} 
			insertHashMap(map, statement->statement.id, stackOffset);
			fprintf(f, "push  %%eax\n");
			stackOffset -= 4;
			//fprintf(f, "xor  %%eax, %%eax\n");
		}
	}
	else {
		generateExpr(statement->statement.expression, f, map);
	}
}

void generateFunction(ASTNode * function, FILE *f, HashMap *map)
{
	fprintf(f, " .globl _%s\n", function->function.name);
	fprintf(f, "_%s:\n", function->function.name);
	fprintf(f, "push  %%ebp\n");
	fprintf(f, "movl  %%esp, %%ebp\n");
	for (int i = 0; i < function->function.statementCount; i++) {
		generateStatement(function->function.body[i], f, map);
	}
	fprintf(f, "movl  %%ebp, %%esp\n");
	fprintf(f, "pop  %%ebp\n");
	fprintf(f, "ret  \n");
}

void generateAssembly(ASTNode * root, FILE *f)
{
	HashMap *map = createNewHashMap(2);
	generateFunction(root->program.children, f, map);
	freeHashMap(map);
}
