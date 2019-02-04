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
	if (statement->type == RETURN_STATEMENT) {
		freeExprAST(statement->statement.returnExpression);
	}
	else if (statement->type == DECLARE_STATEMENT) {
		if (statement->statement.optinalAssignExpression != NULL) {
			freeExprAST(statement->statement.optinalAssignExpression);
		}
	}
	else {
		freeExprAST(statement->statement.expression);
	}
	free(statement);
}

//Write this function
void freeExprAST(ASTNode * expr)
{
	if (expr->type == EXPRESSION) {
		freeLogicalOrExprAST(expr->expression.precedenceExp);
	}
	else if (expr->type == ASSIGN_EXPRESSION) {
		freeExprAST(expr->expression.expression);
	}
	free(expr);
}

void freeLogicalOrExprAST(ASTNode * logicalOrExpr)
{
	if (logicalOrExpr->type == BINARY_OP) {
		freeLogicalOrExprAST(logicalOrExpr->logicalOrExp.rightExp);
	}
	freeLogicalAndAST(logicalOrExpr->logicalOrExp.logicalAndExp);
	free(logicalOrExpr);
}

void freeLogicalAndAST(ASTNode * logAndExpr)
{
	if (logAndExpr->type == BINARY_OP) {
		freeLogicalAndAST(logAndExpr->logicalAndExp.rightLogicalAndExp);
	}
	freeBitwiseOrAST(logAndExpr->logicalAndExp.bitwiseOrExp);
	free(logAndExpr);
}

void freeBitwiseOrAST(ASTNode * bitwiseOrExpr)
{
	if (bitwiseOrExpr->type == BINARY_OP) {
		freeBitwiseOrAST(bitwiseOrExpr->bitwiseOrExp.rightBitwiseOrExp);
	}
	freeBitwiseXorAST(bitwiseOrExpr->bitwiseOrExp.bitwiseXorExp);
	free(bitwiseOrExpr);
}

void freeBitwiseXorAST(ASTNode * bitwiseXorExpr)
{
	if (bitwiseXorExpr->type == BINARY_OP) {
		freeBitwiseXorAST(bitwiseXorExpr->bitwiseXorExp.rightBitwiseXorExp);
	}
	freeBitwiseAndAST(bitwiseXorExpr->bitwiseXorExp.bitwiseAndExp);
	free(bitwiseXorExpr);
}

void freeBitwiseAndAST(ASTNode * bitwiseAndExpr)
{
	if (bitwiseAndExpr->type == BINARY_OP) {
		freeBitwiseAndAST(bitwiseAndExpr->bitwiseAndExp.rightBitwiseAndExp);
	}
	freeEqualityAST(bitwiseAndExpr->bitwiseAndExp.equalityExp);
	free(bitwiseAndExpr);
}

void freeEqualityAST(ASTNode * eqExpr)
{
	if (eqExpr->type == BINARY_OP) {
		freeEqualityAST(eqExpr->equalityExp.rightEqualityExp);
	}
	freeRelationAST(eqExpr->equalityExp.relationalExp);
	free(eqExpr);
}

void freeRelationAST(ASTNode * relaExpr)
{
	if (relaExpr->type == BINARY_OP) {
		freeRelationAST(relaExpr->relationalExp.rightRelationalExp);
	}
	freeBitwiseShiftAST(relaExpr->relationalExp.bitwiseShiftExp);
	free(relaExpr);
}

void freeBitwiseShiftAST(ASTNode * bitwiseShiftExpr)
{
	if (bitwiseShiftExpr->type == BINARY_OP) {
		freeBitwiseShiftAST(bitwiseShiftExpr->bitwiseShiftExp.rightBitwiseShiftExp);
	}
	freeAdditiveAST(bitwiseShiftExpr->bitwiseShiftExp.additiveExp);
	free(bitwiseShiftExpr);
}

void freeAdditiveAST(ASTNode * addExpr)
{
	if (addExpr->type == BINARY_OP) {
		freeAdditiveAST(addExpr->additiveExp.rightAdditiveExp);
	}
	freeTermAST(addExpr->additiveExp.term);
	free(addExpr);
}

void freeTermAST(ASTNode * term)
{
	if (term->type == BINARY_OP) {
		freeTermAST(term->term.rightTerm);
	}
	freeFactorAST(term->term.factor);
	free(term);
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
	else if (factor->type == EXPRESSION) {
		freeLogicalOrExprAST(factor->factor.expr);
	}
	else if (factor->type == VARIABLE) {
		free(factor);
	}
}
