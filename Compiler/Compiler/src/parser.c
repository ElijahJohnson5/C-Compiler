#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "parser.h"
#include "ast.h"

TokenType PRECEDENCE_ORDER[][11] = {
	{LOGICAL_NEGATION, BITWISE_COMPLIMENT, MINUS},
	{MULTIPLICATION, DIVISION, MODULUS},
	{ADDITION, MINUS},
	{BITWISE_SHIFT_LEFT, BITWISE_SHIFT_RIGHT},
	{GREATER_THAN, GREATER_THAN_EQUAL_TO, LESS_THAN, LESS_THAN_EQUAL_TO},
	{EQUAL_TO, NOT_EQUAL_TO},
	{BITWISE_AND},
	{BITWISE_XOR},
	{BITWISE_OR},
	{LOGICAL_AND},
	{LOGICAL_OR},
	{ASSIGNMENT}
};

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
	int statementCount = 0, currentSize = 0;
	ASTNode *function = malloc(sizeof(ASTNode));
	function->type = FUNCTION;
	function->function.body = NULL;
	Token token = (*tokens)->token;
	*tokens = (*tokens)->next;
	if (token.type != KEYWORD && strcmp(token.value.keyword, "int")) {
		freeFunctionAST(function);
		return NULL;
	}

	function->function.returnType = INT;
	token = (*tokens)->token;
	*tokens = (*tokens)->next;
	if (token.type != IDENTIFIER) {
		freeFunctionAST(function);
		return NULL;
	}
	function->function.name = token.value.identifier;
	token = (*tokens)->token;
	*tokens = (*tokens)->next;
	if (token.type != OPEN_PAREN) {
		freeFunctionAST(function);
		return NULL;
	}
	token = (*tokens)->token;
	*tokens = (*tokens)->next;
	if (token.type != CLOSE_PAREN) {
		freeFunctionAST(function);
		return NULL;
	}

	token = (*tokens)->token;
	*tokens = (*tokens)->next;
	if (token.type != OPEN_BRACE) {
		freeFunctionAST(function);
		return NULL;
	}

	ASTNode *statement = NULL;

	while ((statement = parseStatement(tokens)) != NULL) {
		if (function->function.body == NULL || statementCount >= currentSize) {
			currentSize++;
			function->function.body = realloc(function->function.body, sizeof(ASTNode *) * currentSize);
		}
		function->function.body[statementCount++] = statement;
		if (statement->type == RETURN_STATEMENT) {
			break;
		}
	}
	function->function.statementCount = statementCount;

	token = (*tokens)->token;
	*tokens = (*tokens)->next;
	if (token.type != CLOSE_BRACE) {
		freeFunctionAST(function);
		return NULL;
	}
	return function;
}

ASTNode * parseStatement(TokenList** tokens)
{
	ASTNode *statement = malloc(sizeof(ASTNode));
	statement->type = STATEMENT;
	Token token = (*tokens)->token;
	if (token.type == KEYWORD && !strcmp(token.value.keyword, "return")) {
		*tokens = (*tokens)->next;
		statement->type = RETURN_STATEMENT;
		statement->statement.returnExpression = parseExpr(tokens);
	}
	else if (token.type != KEYWORD) {
		statement->statement.expression = parseExpr(tokens);
	}
	else if (token.type == KEYWORD) {
		*tokens = (*tokens)->next;
		token = (*tokens)->token;
		*tokens = (*tokens)->next;
		statement->type = DECLARE_STATEMENT;
		statement->statement.id = token.value.identifier;
		token = (*tokens)->token;
		if (token.type == ASSIGNMENT) {
			*tokens = (*tokens)->next;
			statement->statement.optinalAssignExpression = parseExpr(tokens);
		}
		else {
			statement->statement.optinalAssignExpression = NULL;
		}
	}
	else {
		freeStatementAST(statement);
		return NULL;
	}

	token = (*tokens)->token;
	*tokens = (*tokens)->next;
	if (token.type != SEMICOLON) {
		freeStatementAST(statement);
		return NULL;
	}

	return statement;
}

ASTNode * parseExpr(TokenList ** tokens)
{
	ASTNode *expr = malloc(sizeof(ASTNode));
	expr->type = EXPRESSION;
	Token token = (*tokens)->token;
	if (token.type == IDENTIFIER && (*tokens)->next->token.type == ASSIGNMENT) {
		expr->type = ASSIGN_EXPRESSION;
		expr->expression.id = token.value.identifier;
		*tokens = (*tokens)->next;
		token = (*tokens)->token;
		*tokens = (*tokens)->next;
		expr->expression.op = token.value.token;
		expr->expression.expression = parseExpr(tokens);
		return expr;
	} else {
		expr->expression.precedenceExp = parsePrecedenceExpr(tokens);
		return expr;
	}

	freeExprAST(expr);
	return NULL;
}

ASTNode * parsePrecedenceExpr(TokenList ** tokens)
{
	int precedenceLevel = 11;
	ASTNode *expr = malloc(sizeof(ASTNode));
	expr->type = EXPRESSION;
	expr->precedanceExp.rightExp = NULL;
	expr->precedanceExp.exp = parseFactor(tokens);
	Token next = (*tokens)->token;
	TokenType* typesToCheck = PRECEDENCE_ORDER[precedenceLevel];
	for (int i = 0; i < 11; i++) {
		if (typesToCheck[i] == NONE) {
			if (precedenceLevel - 1 < 0) {
				break;
			}
			typesToCheck = PRECEDENCE_ORDER[--precedenceLevel];
			i = -1;
			continue;
		}
		if (next.type == typesToCheck[i]) {
			(*tokens) = (*tokens)->next;
			expr->type = BINARY_OP;
			if (twoCharTokenType(next.type)) {
				expr->precedanceExp.op[0] = next.value.leftToken;
				expr->precedanceExp.op[1] = next.value.rightToken;
				expr->precedanceExp.op[2] = 0;
			}
			else {
				expr->precedanceExp.op[0] = next.value.token;
				expr->precedanceExp.op[1] = 0;
				expr->precedanceExp.op[2] = 0;
			}
			expr->precedanceExp.rightExp = parsePrecedenceExpr(tokens);
			break;
		}
	}

	return expr;
}

ASTNode * parseFactor(TokenList ** tokens)
{
	ASTNode *factor = malloc(sizeof(ASTNode));
	Token next = (*tokens)->token;
	(*tokens) = (*tokens)->next;
	if (next.type == OPEN_PAREN) {
		//"(" <exp> ")"
		factor->type = EXPRESSION;
		factor->factor.precedenceExpr = parsePrecedenceExpr(tokens);
		next = (*tokens)->token;
		(*tokens) = (*tokens)->next;
		if (next.type != CLOSE_PAREN) {
			freeFactorAST(factor);
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
	else if (next.type == IDENTIFIER) {
		factor->type = VARIABLE;
		factor->factor.id = next.value.identifier;
		return factor;
	}
	freeFactorAST(factor);
	return NULL;
}

void prettyPrintAST(ASTNode * root)
{
	printFunction(root->program.children);
}

void printFunction(ASTNode * function)
{
	printf("FUN %s %s:\n", getReturnType(function->function.returnType), function->function.name);
	printf("\t params: ()\n");
	printf("\t body:\n");
	for (int i = 0; i < function->function.statementCount; i++) {
		printStatement(function->function.body[i]);
	}
}

void printStatement(ASTNode * statement)
{
	if (statement->type == RETURN_STATEMENT) {
		printf("\t  RETURN ");
		printExpr(statement->statement.returnExpression);
	}
	else if (statement->type == DECLARE_STATEMENT) {
		printf("\t  INT<%s> ", statement->statement.id);
		if (statement->statement.optinalAssignExpression != NULL) {
			printf("=");
			printExpr(statement->statement.optinalAssignExpression);
		}
	}
	else {
		printf("\t ");
		printExpr(statement->statement.expression);
	}

	printf("\n");
}

void printExpr(ASTNode * expr)
{
	if (expr->type == EXPRESSION) {
		printPrecedenceExpr(expr->expression.precedenceExp);
	}
	else if (expr->type == ASSIGN_EXPRESSION) {
		printPrecedenceExpr(expr->expression.precedenceExp);
		printf(" INT<%s> %c ", expr->expression.id, expr->expression.op);
		printExpr(expr->expression.expression);
	}
}

void printPrecedenceExpr(ASTNode * precedenceExp)
{
	if (precedenceExp->type == EXPRESSION) {
		printFactor(precedenceExp->precedanceExp.exp);
	}
	if (precedenceExp->type == BINARY_OP) {
		printFactor(precedenceExp->precedanceExp.exp);
		printf(" %s ", precedenceExp->precedanceExp.op);
		printPrecedenceExpr(precedenceExp->precedanceExp.rightExp);
	}
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
	else if (factor->type == VARIABLE) {
		printf(" %s ", factor->factor.id);
	}
	else {
		printf("(");
		printPrecedenceExpr(factor->factor.precedenceExpr);
		printf(")");
	}
} 