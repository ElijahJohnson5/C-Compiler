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
	expr->type = EXPRESSION;
	expr->expression.rightExp = NULL;
	expr->expression.term = parseTerm(tokens);
	Token next = (*tokens)->token;

	if (next.type == ADDITION || next.type == MINUS) {
		(*tokens) = (*tokens)->next;
		expr->type = BINARY_OP;
		
		expr->expression.op = next.value.token;
		expr->expression.rightExp = parseExpr(tokens);
	}

	return expr;
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
		factor->factor.factor.unaryOp = next.value.token;
		factor->factor.factor.factor = parseFactor(tokens);
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
		printf("%c", factor->factor.factor.unaryOp);
		printFactor(factor->factor.factor.factor);
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
		printf("%c", term->term.op);
		printTerm(term->term.rightTerm);
	}
}

void printExpr(ASTNode * expr)
{
	if (expr->type == EXPRESSION) {
		printTerm(expr->expression.term);
	}
	else if (expr->type == BINARY_OP) {
		printTerm(expr->expression.term);
		printf("%c", expr->expression.op);
		printExpr(expr->expression.rightExp);
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