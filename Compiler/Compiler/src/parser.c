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
	int termCount = 0, currentSize = 0;
	ASTNode *expr = malloc(sizeof(ASTNode));
	expr->type = EXPRESSION;
	expr->expression.rightTerm = NULL;
	expr->expression.op = NULL;
	expr->expression.term = parseTerm(tokens);
	Token next = (*tokens)->token;

	while (next.type == ADDITION || next.type == MINUS) {
		if (expr->expression.rightTerm == NULL || termCount == currentSize) {
			currentSize++;
			expr->expression.rightTerm = realloc(expr->expression.rightTerm, sizeof(ASTNode *) * currentSize);
			expr->expression.op = realloc(expr->expression.op, sizeof(char) * currentSize);
		}
		(*tokens) = (*tokens)->next;
		expr->type = BINARY_OP;
		
		expr->expression.rightTerm[termCount] = parseTerm(tokens);
		expr->expression.op[termCount] = next.value.token;
		termCount++;
		next = (*tokens)->token;
	}
	if (expr->type == BINARY_OP) {
		expr->expression.termCount = termCount;
	}

	return expr;
}

ASTNode * parseTerm(TokenList ** tokens)
{
	int factorCount = 0, currentSize = 0;
	ASTNode *term = malloc(sizeof(ASTNode));
	term->type = TERM;
	term->term.rightFactor = NULL;
	term->term.op = NULL;
	term->term.factor = parseFactor(tokens);
	Token next = (*tokens)->token;

	while (next.type == MULTIPLICATION || next.type == DIVISION) {
		if (term->term.rightFactor == NULL || factorCount >= currentSize) {
			currentSize++;
			term->term.rightFactor = realloc(term->term.rightFactor, sizeof(ASTNode *) * currentSize);
			term->term.op = realloc(term->term.op, sizeof(char) * currentSize);
		}
		(*tokens) = (*tokens)->next;
		term->type = BINARY_OP;
		term->term.rightFactor[factorCount] = parseFactor(tokens);
		term->term.op[factorCount] = next.value.token;
		factorCount++;
		next = (*tokens)->token;
	}

	if (term->type == BINARY_OP) {
		term->term.factorCount = factorCount;
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
	if (term->type == TERM) {
		printFactor(term->term.factor);
	} 
	else if (term->type == BINARY_OP) {
		printFactor(term->term.factor);
		for (int i = 0; i < term->term.factorCount; i++) {
			printf("%c", term->term.op[i]);
			printFactor(term->term.rightFactor[i]);
		}
	}
}

void printExpr(ASTNode * expr)
{
	if (expr->type == EXPRESSION) {
		printTerm(expr->expression.term);
	}
	else if (expr->type == BINARY_OP) {
		printTerm(expr->expression.term);
		for (int i = 0; i < expr->expression.termCount; i++) {
			printf("%c", expr->expression.op[i]);
			printTerm(expr->expression.rightTerm[i]);
		}
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