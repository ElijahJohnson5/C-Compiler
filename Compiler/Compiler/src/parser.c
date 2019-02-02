#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "parser.h"
#include "ast.h"

ASTNode * parseProgram(TokenList** tokens)
{
	ASTNode *root = malloc(sizeof(ASTNode));
	root->type = PROGRAM;
	root->program.count = 1;
	root->program.children = parseFunction(tokens);
	
	return root;
}

ASTNode * parseFunction(TokenList** tokens)
{
	ASTNode *function = malloc(sizeof(ASTNode));
	function->type = FUNCTION;
	Token token = (*tokens)->token;
	*tokens = (*tokens)->next;
	if (token.type != KEYWORD && strcmp(token.value.keyword, "int")) {
		free(function);
		return NULL;
	}

	function->function.returnType = INT;
	token = (*tokens)->token;
	*tokens = (*tokens)->next;
	if (token.type != IDENTIFIER) {
		free(function);
		return NULL;
	}
	function->function.name = token.value.identifier;
	token = (*tokens)->token;
	*tokens = (*tokens)->next;
	if (token.type != OPEN_PAREN) {
		free(function);
		return NULL;
	}
	token = (*tokens)->token;
	*tokens = (*tokens)->next;
	if (token.type != CLOSE_PAREN) {
		free(function);
		return NULL;
	}

	token = (*tokens)->token;
	*tokens = (*tokens)->next;
	if (token.type != OPEN_BRACE) {
		free(function);
		return NULL;
	}

	function->function.body = parseStatement(tokens);

	token = (*tokens)->token;
	*tokens = (*tokens)->next;
	if (token.type != CLOSE_BRACE) {
		free(function);
		return NULL;
	}
	return function;
}

ASTNode * parseStatement(TokenList** tokens)
{
	ASTNode *statement = malloc(sizeof(ASTNode));
	statement->type = STATEMENT;
	Token token = (*tokens)->token;
	*tokens = (*tokens)->next;
	if (token.type != KEYWORD && strcmp(token.value.keyword, "return")) {
		free(statement);
		return NULL;
	}

	statement->statement.expression = parseExpr(tokens);

	token = (*tokens)->token;
	*tokens = (*tokens)->next;
	if (token.type != SEMICOLON) {
		free(statement);
		return NULL;
	}

	return statement;
}

ASTNode * parseExpr(TokenList ** tokens)
{
	ASTNode *expr = malloc(sizeof(ASTNode));
	expr->type = EXPRESSION;
	expr->expression.rightExp = NULL;
	expr->expression.logicalAndExp = parseLogicalAndExpr(tokens);
	Token next = (*tokens)->token;

	if (next.type == LOGICAL_OR) {
		(*tokens) = (*tokens)->next;
		expr->type = BINARY_OP;
		expr->expression.op[0] = next.value.leftToken;
		expr->expression.op[1] = next.value.rightToken;
		expr->expression.op[2] = 0;
		expr->expression.rightExp = parseExpr(tokens);
	}

	return expr;
}

ASTNode * parseLogicalAndExpr(TokenList ** tokens)
{
	ASTNode *logicalAndExpr = malloc(sizeof(ASTNode));
	logicalAndExpr->type = EXPRESSION;
	logicalAndExpr->logicalAndExp.rightLogicalAndExp = NULL;
	logicalAndExpr->logicalAndExp.equalityExp = parseEqualityExpr(tokens);
	Token next = (*tokens)->token;

	if (next.type == LOGICAL_AND) {
		(*tokens) = (*tokens)->next;
		logicalAndExpr->type = BINARY_OP;
		logicalAndExpr->logicalAndExp.op[0] = next.value.leftToken;
		logicalAndExpr->logicalAndExp.op[1] = next.value.rightToken;
		logicalAndExpr->logicalAndExp.op[2] = 0;
		logicalAndExpr->logicalAndExp.rightLogicalAndExp = parseLogicalAndExpr(tokens);
	}

	return logicalAndExpr;
}

ASTNode * parseEqualityExpr(TokenList ** tokens)
{
	ASTNode *equalityExpr = malloc(sizeof(ASTNode));
	equalityExpr->type = EXPRESSION;
	equalityExpr->equalityExp.rightEqualityExp = NULL;
	equalityExpr->equalityExp.relationalExp = parseRelationalExpr(tokens);
	Token next = (*tokens)->token;

	if (next.type == NOT_EQUAL_TO || next.type == EQUAL_TO) {
		(*tokens) = (*tokens)->next;
		equalityExpr->type = BINARY_OP;
		equalityExpr->equalityExp.op[0] = next.value.leftToken;
		equalityExpr->equalityExp.op[1] = next.value.rightToken;
		equalityExpr->equalityExp.op[2] = 0;
		equalityExpr->equalityExp.rightEqualityExp = parseEqualityExpr(tokens);
	}

	return equalityExpr;
}

ASTNode * parseRelationalExpr(TokenList ** tokens)
{
	ASTNode *relationalExpr = malloc(sizeof(ASTNode));
	relationalExpr->type = EXPRESSION;
	relationalExpr->relationalExp.rightRelationalExp = NULL;
	relationalExpr->relationalExp.additiveExp = parseAdditiveExpr(tokens);
	Token next = (*tokens)->token;

	if (next.type == GREATER_THAN || next.type == GREATER_THAN_EQUAL_TO || next.type == LESS_THAN || next.type == LESS_THAN_EQUAL_TO) {
		(*tokens) = (*tokens)->next;
		relationalExpr->type = BINARY_OP;
		relationalExpr->relationalExp.op[0] = next.value.leftToken;
		relationalExpr->relationalExp.op[1] = next.value.rightToken;
		relationalExpr->relationalExp.op[2] = 0;
		relationalExpr->relationalExp.rightRelationalExp = parseRelationalExpr(tokens);
	}

	return relationalExpr;
}

ASTNode * parseAdditiveExpr(TokenList ** tokens)
{
	ASTNode *additiveExpr = malloc(sizeof(ASTNode));
	additiveExpr->type = EXPRESSION;
	additiveExpr->additiveExp.rightAdditiveExp = NULL;
	additiveExpr->additiveExp.term = parseTerm(tokens);
	Token next = (*tokens)->token;

	if (next.type == ADDITION || next.type == MINUS) {
		(*tokens) = (*tokens)->next;
		additiveExpr->type = BINARY_OP;
		additiveExpr->additiveExp.op = next.value.token;
		additiveExpr->additiveExp.rightAdditiveExp = parseAdditiveExpr(tokens);
	}

	return additiveExpr;
}

ASTNode * parseTerm(TokenList ** tokens)
{
	int factorCount = 0, currentSize = 0;
	ASTNode *term = malloc(sizeof(ASTNode));
	term->type = TERM;
	term->term.rightTerm = NULL;
	term->term.factor = parseFactor(tokens);
	Token next = (*tokens)->token;

	if (next.type == MULTIPLICATION || next.type == DIVISION) {
		//<factor> {('*') | ('/') <factor>}
		(*tokens) = (*tokens)->next;
		term->type = BINARY_OP;
		term->term.op = next.value.token;
		term->term.rightTerm = parseTerm(tokens);
	}
	return term;
}

ASTNode * parseFactor(TokenList ** tokens)
{
	ASTNode *factor = malloc(sizeof(ASTNode));
	Token next = (*tokens)->token;
	(*tokens) = (*tokens)->next;
	if (next.type == OPEN_PAREN) {
		//"(" <exp> ")"
		factor->type = EXPRESSION;
		factor->factor.expression = parseExpr(tokens);
		next = (*tokens)->token;
		(*tokens) = (*tokens)->next;
		if (next.type != CLOSE_PAREN) {
			return NULL;
		}
		return factor;
	}
	else if (isUnOp(next)) {
		//<unary_op> <factor>
		ASTNode *factor = malloc(sizeof(ASTNode));
		factor->type = UNARY_OPERATOR;
		factor->factor.unaryOp = next.value.token;
		factor->factor.factor = parseFactor(tokens);
		return factor;
	}
	else if (next.type == INT_LITERAL) {
		//<int>
		factor->type = CONSTANT_INT;
		factor->value = next.value.intLiteral;
		return factor;
	}

	return NULL;
}



void printFactor(ASTNode * factor)
{
	if (factor->type == CONSTANT_INT) {
		printf("Int<%d>", factor->value);
	}
	else if (factor->type == UNARY_OPERATOR) {
		printf(" %c ", factor->factor.unaryOp);
		printFactor(factor->factor.factor);
	}
	else {
		printf("(");
		printExpr(factor->factor.expression);
		printf(")");
	}
}

void printTerm(ASTNode * term)
{
	if (term->type == TERM) {
		printFactor(term->term.factor);
	} 
	else if (term->type == BINARY_OP) {
		printFactor(term->term.factor);
		printf(" %c ", term->term.op);
		printTerm(term->term.rightTerm);
	}
}

void printAdditiveExpr(ASTNode * addExpr)
{
	if (addExpr->type == EXPRESSION) {
		printTerm(addExpr->additiveExp.term);
	}
	else if (addExpr->type == BINARY_OP) {
		printTerm(addExpr->additiveExp.term);
		printf(" %c ", addExpr->additiveExp.op);
		printAdditiveExpr(addExpr->additiveExp.rightAdditiveExp);
	}
}

void printRelationalExpr(ASTNode * relaExpr)
{
	if (relaExpr->type == EXPRESSION) {
		printAdditiveExpr(relaExpr->relationalExp.additiveExp);
	}
	else if (relaExpr->type == BINARY_OP) {
		printAdditiveExpr(relaExpr->relationalExp.additiveExp);
		printf(" %s ", relaExpr->relationalExp.op);
		printRelationalExpr(relaExpr->relationalExp.rightRelationalExp);
	}
}

void printEqualityExpr(ASTNode * eqExpr)
{
	if (eqExpr->type == EXPRESSION) {
		printRelationalExpr(eqExpr->equalityExp.relationalExp);
	}
	else if (eqExpr->type == BINARY_OP) {
		printRelationalExpr(eqExpr->equalityExp.relationalExp);
		printf(" %s ", eqExpr->equalityExp.op);
		printEqualityExpr(eqExpr->equalityExp.rightEqualityExp);
	}
}

void printLogicalAndExpr(ASTNode * logAndExpr)
{
	if (logAndExpr->type == EXPRESSION) {
		printEqualityExpr(logAndExpr->logicalAndExp.equalityExp);
	}
	else if (logAndExpr->type == BINARY_OP) {
		printEqualityExpr(logAndExpr->logicalAndExp.equalityExp);
		printf(" %s ", logAndExpr->logicalAndExp.op);
		printLogicalAndExpr(logAndExpr->logicalAndExp.rightLogicalAndExp);
	}
}

void printExpr(ASTNode * expr)
{
	if (expr->type == EXPRESSION) {
		printLogicalAndExpr(expr->expression.logicalAndExp);
	}
	else if (expr->type == BINARY_OP) {
		printLogicalAndExpr(expr->expression.logicalAndExp);
		printf(" %s ", expr->expression.op);
		printExpr(expr->expression.rightExp);
	}

}

void printStatement(ASTNode * statement)
{
	printf("\t  RETURN ");
	printExpr(statement->statement.expression);
	printf("\n");
}

void printFunction(ASTNode * function)
{
	printf("FUN %s %s:\n", getReturnType(function->function.returnType), function->function.name);
	printf("\t params: ()\n");
	printf("\t body:\n");
	printStatement(function->function.body);
}

void prettyPrintAST(ASTNode * root)
{
	printFunction(root->program.children);
}