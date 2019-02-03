#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

const char * getReturnType(ReturnType r)
{
	switch (r) {
	case INT: return "INT";
	}

	return NULL;
}

void freeAST(ASTNode * root)
{
	freeFunctionAST(root->program.children);
	free(root);
}

//Update for variable amount of statements
void freeFunctionAST(ASTNode * function)
{
	for (int i = 0; i < function->function.statementCount; i++) {
		freeStatementAST(function->function.body[i]);
	}
	free(function->function.body);
	free(function);
}

//Update function to use freeExpr
void freeStatementAST(ASTNode * statement)
{
	freeExprAST(statement->statement.expression);
	free(statement);
}

//Write this function
void freeExprAST(ASTNode * expr)
{
	if (expr->type == EXPRESSION) {
		freePrecedenceExprAST(expr->expression.precedenceExp);
	}
	else if (expr->type == ASSIGN_EXPRESSION) {
		freeExprAST(expr->expression.expression);
	}
	free(expr);
}

void freePrecedenceExprAST(ASTNode * precedenceExpr)
{
	freeFactorAST(precedenceExpr->precedanceExp.exp);
	if (precedenceExpr->type == BINARY_OP) {
		freePrecedenceExprAST(precedenceExpr->precedanceExp.rightExp);
	}
	free(precedenceExpr);
}


//Update function for variable types
void freeFactorAST(ASTNode * factor)
{
	if (factor->type == CONSTANT_INT) {
		free(factor);
	}
	else if (factor->type == UNARY_OPERATOR) {
		freeFactorAST(factor->factor.factor);
	}
	else {
		freePrecedenceExprAST(factor->factor.precedenceExpr);
	}
}
