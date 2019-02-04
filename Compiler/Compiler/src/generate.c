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
		generatePrecedenceExpr(factor->factor.precedenceExpr, f, map);
	}
	else if (factor->type == VARIABLE) {
		fprintf(f, "movl  %d(%%ebp), %%eax\n", lookupNode(map, factor->factor.id));
	}
}

void generateOpAssembly(char * op, FILE * f, HashMap *map)
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

void generatePrecedenceExpr(ASTNode * precedenceExpr, FILE * f, HashMap *map)
{
	if (precedenceExpr->type == BINARY_OP) {
		generatePrecedenceExpr(precedenceExpr->precedanceExp.rightExp, f, map);
		fprintf(f, "push  %%eax\n");
	}
	generateFactor(precedenceExpr->precedanceExp.exp, f, map);

	generateOpAssembly(precedenceExpr->precedanceExp.op, f, map);
}

void generateExpr(ASTNode * expr, FILE *f, HashMap *map)
{
	if (expr->type == ASSIGN_EXPRESSION) {
		generateExpr(expr->expression.expression, f, map);
		fprintf(f, "movl  %%eax, %d(%%ebp)\n", lookupNode(map, expr->expression.id));
	}
	else {
		generatePrecedenceExpr(expr->expression.precedenceExp, f, map);
	}
}

void generateStatement(ASTNode * statement, FILE *f, HashMap *map)
{
	if (statement->type == DECLARE_STATEMENT) {
		if (lookupNode(map, statement->statement.id) != -1) {
			return;
		}
		else {
			if (statement->statement.optinalAssignExpression != NULL) {
				generateExpr(statement->statement.optinalAssignExpression, f, map);
			} 
			insertHashMap(map, statement->statement.id, stackOffset);
			stackOffset -= 4;
			fprintf(f, "push  %%eax\n");
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
}
