#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "parser.h"

const char * getReturnType(ReturnType r)
{
	switch (r) {
	case INT: return "INT";
	}
}

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
	
	Token token = (*tokens)->token;
	*tokens = (*tokens)->next;
	if (token.type == INT_LITERAL) {
		expr->value = token.value.intLiteral;
		expr->type = CONSTANT_EXPRESSION;
		return expr;
	}
	else {
		expr->type = UNARY_OPERATOR;
		expr->expression.unaryOp = token.value.token;
		expr->expression.expression = parseExpr(tokens);
		return expr;
	}
	
	free(expr);
	return NULL;
}

void printExpr(ASTNode * expr)
{
	if (expr->type == CONSTANT_EXPRESSION) {
		printf("Int<%d>\n", expr->value);
	}
	else {
		printf("%c", expr->expression.unaryOp);
		printExpr(expr->expression.expression);
	}
}

void printStatement(ASTNode * statement)
{
	printf("\t  RETURN ");
	printExpr(statement->statement.expression);
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