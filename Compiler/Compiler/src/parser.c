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
	int termCount = 0, currentSize = 1;
	ASTNode *expr = malloc(sizeof(ASTNode));
	expr->expression.term = malloc(sizeof(ASTNode *));
	expr->expression.op = NULL;
	
	expr->expression.term[termCount] = parseTerm(tokens);
	termCount++;
	Token next = (*tokens)->token;

	while (next.type == ADDITION || next.type == MINUS) {
		(*tokens) = (*tokens)->next;
		if (currentSize == termCount) {
			currentSize = termCount + 1;
			expr->expression.term = realloc(expr->expression.term, sizeof(ASTNode *) * currentSize);
			expr->expression.op = realloc(expr->expression.op, sizeof(char) * termCount);
		}
		expr->expression.term[termCount] = parseTerm(tokens);
		expr->expression.op[termCount - 1] = next.value.token;
		termCount++;
		next = (*tokens)->token;
	}
	expr->expression.termCount = termCount;
	return expr;
}

ASTNode * parseTerm(TokenList ** tokens)
{
	int factorCount = 0, currentSize = 1;
	ASTNode *term = malloc(sizeof(ASTNode));
	term->term.factor = malloc(sizeof(ASTNode *));
	term->term.op = NULL;

	term->term.factor[factorCount] = parseFactor(tokens);
	factorCount++;
	Token next = (*tokens)->token;

	while (next.type == MULTIPLICATION || next.type == DIVISION) {
		(*tokens) = (*tokens)->next;
		if (currentSize == factorCount) {
			currentSize = factorCount + 1;
			term->term.factor = realloc(term->term.factor, sizeof(ASTNode *) * currentSize);
			term->term.op = realloc(term->term.op, sizeof(char) * factorCount);
		}
		term->term.factor[factorCount] = parseFactor(tokens);
		term->term.op[factorCount - 1] = next.value.token;
		factorCount++;
		next = (*tokens)->token;
	}
	term->term.factorCount = factorCount;
	return term;
}

ASTNode * parseFactor(TokenList ** tokens)
{
	ASTNode *factor = malloc(sizeof(ASTNode));
	Token next = (*tokens)->token;
	(*tokens) = (*tokens)->next;
	if (next.type == OPEN_PAREN) {
		//"(" <exp> ")"
		factor->factor.expression = parseExpr(tokens);
		next = (*tokens)->token;
		(*tokens) = (*tokens)->next;
		if (next.type != CLOSE_PAREN) {
			return NULL;
		}
		return factor;
	}
	else if (isUnOp(next)) {
		ASTNode *factor = malloc(sizeof(ASTNode));
		factor->type = UNARY_OPERATOR;
		factor->factor.factor.unaryOp = next.value.token;
		factor->factor.factor.factor = parseFactor(tokens);
		return factor;
	}
	else if (next.type == INT_LITERAL) {
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
	for (int i = 0; i < term->term.factorCount; i++) {
		if (term->term.op != NULL && i - 1 >= 0) {
			printf("%c", term->term.op[i - 1]);
		}
		printFactor(term->term.factor[i]);
	}
}

void printExpr(ASTNode * expr)
{
	for (int i = 0; i < expr->expression.termCount; i++) {
		if (expr->expression.op != NULL && i - 1 >= 0) {
			printf("%c", expr->expression.op[i - 1]);
		}
		printTerm(expr->expression.term[i]);
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