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
		return NULL;
	}

	function->function.returnType = INT;
	token = (*tokens)->token;
	*tokens = (*tokens)->next;
	if (token.type != IDENTIFIER) {
		return NULL;
	}
	function->function.name = token.value.identifier;
	token = (*tokens)->token;
	*tokens = (*tokens)->next;
	if (token.type != OPEN_PAREN) {
		return NULL;
	}
	token = (*tokens)->token;
	*tokens = (*tokens)->next;
	if (token.type != CLOSE_PAREN) {
		return NULL;
	}

	token = (*tokens)->token;
	*tokens = (*tokens)->next;
	if (token.type != OPEN_BRACE) {
		return NULL;
	}

	function->function.body = parseStatement(tokens);

	token = (*tokens)->token;
	*tokens = (*tokens)->next;
	if (token.type != CLOSE_BRACE) {
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
		return NULL;
	}

	statement->returnStatement.expression = parseExpr(tokens);

	token = (*tokens)->token;
	*tokens = (*tokens)->next;
	if (token.type != SEMICOLON) {
		return NULL;
	}

	return statement;
}

ASTNode * parseExpr(TokenList ** tokens)
{
	ASTNode *expr = malloc(sizeof(ASTNode));
	expr->type = EXPRESSION;
	Token token = (*tokens)->token;
	*tokens = (*tokens)->next;
	if (token.type != LITERAL) {
		return NULL;
	}

	expr->value = token.value.intLiteral;

	return expr;
}

void printExpr(ASTNode * expr)
{
	printf("Int<%d>\n", expr->value);
}

void printStatement(ASTNode * statement)
{
	printf("\t\t RETURN ");
	printExpr(statement->returnStatement.expression);
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