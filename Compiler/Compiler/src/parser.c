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

	ASTNode *blockItem = NULL;

	while ((blockItem = parseBlockItem(tokens)) != NULL) {
		if (function->function.body == NULL || statementCount >= currentSize) {
			currentSize++;
			function->function.body = realloc(function->function.body, sizeof(ASTNode *) * currentSize);
		}
		function->function.body[statementCount++] = blockItem;
		token = (*tokens)->token;
		if (token.type == CLOSE_BRACE) {
			break;
		}
	}
	function->function.blockItemCount = statementCount;

	token = (*tokens)->token;
	*tokens = (*tokens)->next;
	if (token.type != CLOSE_BRACE) {
		freeFunctionAST(function);
		return NULL;
	}
	return function;
}

ASTNode * parseBlockItem(TokenList ** tokens)
{
	ASTNode *blockItem = malloc(sizeof(ASTNode));
	blockItem->type = BLOCK_ITEM;
	Token token = (*tokens)->token;
	if (token.type == KEYWORD) {
		if (!strcmp(token.value.keyword, "if") || !strcmp(token.value.keyword, "return")) {
			blockItem->type = STATEMENT;
			blockItem->blockItem.statement = parseStatement(tokens);
		}
		else if (!strcmp(token.value.keyword, "int")) {
			blockItem->type = DECLARATION;
			blockItem->blockItem.declaration = parseDeclaration(tokens);
		}
	}
	else {
		blockItem->type = STATEMENT;
		blockItem->blockItem.statement = parseStatement(tokens);
	}

	return blockItem;
}

ASTNode * parseDeclaration(TokenList ** tokens)
{
	ASTNode *declaration = malloc(sizeof(ASTNode));
	declaration->type = DECLARATION;
	Token token = (*tokens)->token;
	if (token.type == KEYWORD) {
		*tokens = (*tokens)->next;
		token = (*tokens)->token;
		*tokens = (*tokens)->next;
		declaration->type = DECLARATION;
		declaration->declaration.id = token.value.identifier;
		token = (*tokens)->token;
		if (token.type == ASSIGNMENT) {
			*tokens = (*tokens)->next;
			declaration->declaration.optionalAssignExpression = parseExpr(tokens);
		}
		else {
			declaration->declaration.optionalAssignExpression = NULL;
		}
	}

	token = (*tokens)->token;
	*tokens = (*tokens)->next;
	if (token.type != SEMICOLON) {
		//freeDeclarationAST(declaration);
		return NULL;
	}

	return declaration;
}

ASTNode * parseStatement(TokenList** tokens)
{
	ASTNode *statement = malloc(sizeof(ASTNode));
	statement->type = STATEMENT;
	Token token = (*tokens)->token;
	if (token.type == KEYWORD) {
		if (!strcmp(token.value.keyword, "return")) {
			*tokens = (*tokens)->next;
			statement->type = RETURN_STATEMENT;
			statement->statement.returnExpression = parseExpr(tokens);
		}
		else if (!strcmp(token.value.keyword, "if")) {
			statement->type = IF_STATEMENT;
			*tokens = (*tokens)->next;
			token = (*tokens)->token;
			if (token.type != OPEN_PAREN) {
				freeStatementAST(statement);
				return NULL;
			}
			*tokens = (*tokens)->next;
			statement->statement.exp = parseExpr(tokens);

			token = (*tokens)->token;
			if (token.type != CLOSE_PAREN) {
				freeStatementAST(statement);
				return NULL;
			}
			*tokens = (*tokens)->next;
			statement->statement.statement = parseStatement(tokens);
			token = (*tokens)->token;
			if (token.type == KEYWORD && !strcmp(token.value.keyword, "else")) {
				*tokens = (*tokens)->next;
				statement->statement.optionalElse = parseStatement(tokens);
			}
			else {
				statement->statement.optionalElse = NULL;
			}

			return statement;
		}
	}
	else if (token.type != KEYWORD) {
		statement->statement.expression = parseExpr(tokens);
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
	if (token.type == IDENTIFIER && ((*tokens)->next->token.type == ASSIGNMENT || (*tokens)->next->next->token.type == ASSIGNMENT)) {
		switch ((*tokens)->next->token.type) {
		case ASSIGNMENT:
			expr->type = ASSIGN_EXPRESSION;
			expr->expression.id = token.value.identifier;
			*tokens = (*tokens)->next;
			token = (*tokens)->token;
			*tokens = (*tokens)->next;
			expr->expression.op[0] = token.value.token[0];
			expr->expression.op[1] = 0;
			expr->expression.op[2] = 0;
			expr->expression.op[3] = 0;
			expr->expression.expression = parseExpr(tokens);
			break;
		case ADDITION:
		case MINUS:
		case DIVISION:
		case MULTIPLICATION:
		case MODULUS:
		case BITWISE_AND:
		case BITWISE_OR:
		case BITWISE_XOR:
			if ((*tokens)->next->next->token.type == ASSIGNMENT) {
				TokenList *firstNewNode = createTokenList();
				firstNewNode->token.type = token.type;
				firstNewNode->token.value.identifier = token.value.identifier;
				TokenList *assignment = createTokenList();
				assignment->token.type = (*tokens)->next->next->token.type;
				assignment->token.value.token[0] = (*tokens)->next->next->token.value.token[0];
				assignment->token.value.token[1] = 0;
				TokenList *secondnewNode = createTokenList();
				secondnewNode->token.type = token.type;
				secondnewNode->token.value.identifier = token.value.identifier;
				TokenList *thirdNewNode = createTokenList();
				thirdNewNode->token.type = (*tokens)->next->token.type;
				thirdNewNode->token.value.token[0] = (*tokens)->next->token.value.token[0];
				thirdNewNode->token.value.token[1] = 0;
				expr->type = COMPOUND_ASSIGN_EXPRESSION;
				expr->expression.id = token.value.identifier;
				*tokens = (*tokens)->next;
				token = (*tokens)->token;
				expr->expression.op[0] = token.value.token[0];
				*tokens = (*tokens)->next;
				token = (*tokens)->token;
				expr->expression.op[1] = token.value.token[0];
				expr->expression.op[2] = 0;
				expr->expression.op[3] = 0;
				firstNewNode->next = assignment;
				assignment->next = secondnewNode;
				secondnewNode->next = thirdNewNode;
				thirdNewNode->next = (*tokens)->next;
				(*tokens)->next = firstNewNode;
				*tokens = (*tokens)->next;
				expr->expression.expression = parseExpr(tokens);
			}
			break;
		case BITWISE_SHIFT_LEFT:
		case BITWISE_SHIFT_RIGHT:
			if ((*tokens)->next->next->token.type == ASSIGNMENT) {
				TokenList *firstNewNode = createTokenList();
				firstNewNode->token.type = token.type;
				firstNewNode->token.value.identifier = token.value.identifier;
				TokenList *assignment = createTokenList();
				assignment->token.type = (*tokens)->next->next->token.type;
				assignment->token.value.token[0] = (*tokens)->next->next->token.value.token[0];
				assignment->token.value.token[1] = 0;
				TokenList *secondnewNode = createTokenList();
				secondnewNode->token.type = token.type;
				secondnewNode->token.value.identifier = token.value.identifier;
				TokenList *thirdNewNode = createTokenList();
				thirdNewNode->token.type = (*tokens)->next->token.type;
				thirdNewNode->token.value.token[0] = (*tokens)->next->token.value.token[0];
				thirdNewNode->token.value.token[1] = (*tokens)->next->token.value.token[1];
				thirdNewNode->token.value.token[2] = 0;
				expr->type = COMPOUND_ASSIGN_EXPRESSION;
				expr->expression.id = token.value.identifier;
				*tokens = (*tokens)->next;
				token = (*tokens)->token;
				expr->expression.op[0] = token.value.token[0];
				expr->expression.op[1] = token.value.token[1];
				*tokens = (*tokens)->next;
				token = (*tokens)->token;
				expr->expression.op[2] = token.value.token[0];
				expr->expression.op[3] = 0;
				firstNewNode->next = assignment;
				assignment->next = secondnewNode;
				secondnewNode->next = thirdNewNode;
				thirdNewNode->next = (*tokens)->next;
				(*tokens)->next = firstNewNode;
				*tokens = (*tokens)->next;
				expr->expression.expression = parseExpr(tokens);
			}
			break;
		}
		return expr;
	} else {
		expr->expression.precedenceExp = parseLogicalOrExpr(tokens);
		return expr;
	}

	freeExprAST(expr);
	return NULL;
}

ASTNode * parseLogicalOrExpr(TokenList ** tokens)
{
	ASTNode *expr = malloc(sizeof(ASTNode));
	expr->type = EXPRESSION;
	expr->logicalOrExp.rightExp = NULL;
	expr->logicalOrExp.logicalAndExp = parseLogicalAndExpr(tokens);
	Token next = (*tokens)->token;

	if (next.type == LOGICAL_OR) {
		(*tokens) = (*tokens)->next;
		expr->type = BINARY_OP;
		expr->logicalOrExp.op[0] = next.value.token[0];
		expr->logicalOrExp.op[1] = next.value.token[1];
		expr->logicalOrExp.op[2] = 0;
		expr->logicalOrExp.rightExp = parseLogicalOrExpr(tokens);
	}

	return expr;
}

ASTNode * parseLogicalAndExpr(TokenList ** tokens)
{
	ASTNode *logicalAndExpr = malloc(sizeof(ASTNode));
	logicalAndExpr->type = EXPRESSION;
	logicalAndExpr->logicalAndExp.rightLogicalAndExp = NULL;
	logicalAndExpr->logicalAndExp.bitwiseOrExp = parseBitwiseOrExpr(tokens);
	Token next = (*tokens)->token;

	if (next.type == LOGICAL_AND) {
		(*tokens) = (*tokens)->next;
		logicalAndExpr->type = BINARY_OP;
		logicalAndExpr->logicalAndExp.op[0] = next.value.token[0];
		logicalAndExpr->logicalAndExp.op[1] = next.value.token[1];
		logicalAndExpr->logicalAndExp.op[2] = 0;
		logicalAndExpr->logicalAndExp.rightLogicalAndExp = parseLogicalAndExpr(tokens);
	}

	return logicalAndExpr;
}

ASTNode * parseBitwiseOrExpr(TokenList ** tokens)
{
	ASTNode *orExpr = malloc(sizeof(ASTNode));
	orExpr->type = EXPRESSION;
	orExpr->bitwiseOrExp.rightBitwiseOrExp = NULL;
	orExpr->bitwiseOrExp.bitwiseXorExp = parseBitwiseXorExpr(tokens);
	Token next = (*tokens)->token;

	if (next.type == BITWISE_OR) {
		(*tokens) = (*tokens)->next;
		orExpr->type = BINARY_OP;
		orExpr->bitwiseOrExp.op = next.value.token[0];
		orExpr->bitwiseOrExp.rightBitwiseOrExp = parseBitwiseOrExpr(tokens);
	}

	return orExpr;
}

ASTNode * parseBitwiseXorExpr(TokenList ** tokens)
{
	ASTNode *xorExpr = malloc(sizeof(ASTNode));
	xorExpr->type = EXPRESSION;
	xorExpr->bitwiseXorExp.rightBitwiseXorExp = NULL;
	xorExpr->bitwiseXorExp.bitwiseAndExp = parseBitwiseAndExpr(tokens);
	Token next = (*tokens)->token;

	if (next.type == BITWISE_XOR) {
		(*tokens) = (*tokens)->next;
		xorExpr->type = BINARY_OP;
		xorExpr->bitwiseXorExp.op = next.value.token[0];
		xorExpr->bitwiseXorExp.rightBitwiseXorExp = parseBitwiseXorExpr(tokens);
	}

	return xorExpr;
}

ASTNode * parseBitwiseAndExpr(TokenList ** tokens)
{
	ASTNode *andExpr = malloc(sizeof(ASTNode));
	andExpr->type = EXPRESSION;
	andExpr->bitwiseAndExp.rightBitwiseAndExp = NULL;
	andExpr->bitwiseAndExp.equalityExp = parseEqualityExpr(tokens);
	Token next = (*tokens)->token;

	if (next.type == BITWISE_AND) {
		(*tokens) = (*tokens)->next;
		andExpr->type = BINARY_OP;
		andExpr->bitwiseAndExp.op = next.value.token[0];
		andExpr->bitwiseAndExp.rightBitwiseAndExp = parseBitwiseAndExpr(tokens);
	}

	return andExpr;
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
		equalityExpr->equalityExp.op[0] = next.value.token[0];
		equalityExpr->equalityExp.op[1] = next.value.token[1];
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
	relationalExpr->relationalExp.bitwiseShiftExp = parseBitwiseShiftExpr(tokens);
	Token next = (*tokens)->token;

	if (next.type == GREATER_THAN || next.type == GREATER_THAN_EQUAL_TO || next.type == LESS_THAN || next.type == LESS_THAN_EQUAL_TO) {
		(*tokens) = (*tokens)->next;
		relationalExpr->type = BINARY_OP;
		relationalExpr->relationalExp.op[0] = next.value.token[0];
		relationalExpr->relationalExp.op[1] = next.value.token[1];
		relationalExpr->relationalExp.op[2] = 0;
		relationalExpr->relationalExp.rightRelationalExp = parseRelationalExpr(tokens);
	}

	return relationalExpr;
}

ASTNode * parseBitwiseShiftExpr(TokenList ** tokens)
{
	ASTNode *shiftExpr = malloc(sizeof(ASTNode));
	shiftExpr->type = EXPRESSION;
	shiftExpr->bitwiseShiftExp.rightBitwiseShiftExp = NULL;
	shiftExpr->bitwiseShiftExp.additiveExp = parseAdditiveExpr(tokens);
	Token next = (*tokens)->token;

	if (next.type == BITWISE_SHIFT_RIGHT || next.type == BITWISE_SHIFT_LEFT) {
		(*tokens) = (*tokens)->next;
		shiftExpr->type = BINARY_OP;
		shiftExpr->relationalExp.op[0] = next.value.token[0];
		shiftExpr->relationalExp.op[1] = next.value.token[1];
		shiftExpr->relationalExp.op[2] = 0;
		shiftExpr->relationalExp.rightRelationalExp = parseBitwiseShiftExpr(tokens);
	}

	return shiftExpr;
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
		additiveExpr->additiveExp.op = next.value.token[0];
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

	if (next.type == MULTIPLICATION || next.type == DIVISION || next.type == MODULUS) {
		//<factor> {('*') | ('/') | ('%') <factor>}
		(*tokens) = (*tokens)->next;
		term->type = BINARY_OP;
		term->term.op = next.value.token[0];
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
		factor->factor.expr = parseLogicalOrExpr(tokens);
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
		factor->factor.unaryOp = next.value.token[0];
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
	for (int i = 0; i < function->function.blockItemCount; i++) {
		printBlockItem(function->function.body[i]);
	}
}

void printBlockItem(ASTNode * blockItem)
{
	if (blockItem->type == STATEMENT) {
		printStatement(blockItem->blockItem.statement);
	}
	else if (blockItem->type == DECLARATION) {
		printDeclaration(blockItem->blockItem.declaration);
	}
	printf("\n");
}

void printDeclaration(ASTNode * declaration)
{
	if (declaration->type == DECLARATION) {
		printf("\t  INT<%s>", declaration->declaration.id);
		if (declaration->declaration.optionalAssignExpression) {
			printf(" = ");
			printExpr(declaration->declaration.optionalAssignExpression);
		}
		printf("\n");
	}
}

void printStatement(ASTNode * statement)
{
	if (statement->type == RETURN_STATEMENT) {
		printf(" RETURN ");
		printExpr(statement->statement.returnExpression);
	}
	else if (statement->type == IF_STATEMENT) {
		printf("\t  IF (");
		printExpr(statement->statement.exp);
		printf(")");
		printf("\n");
		printf("\t");
		printStatement(statement->statement.statement);
		if (statement->statement.optionalElse) {
			printf("\n\t  ELSE ");
			printStatement(statement->statement.optionalElse);
		}
		
	}
	else {
		printf("\t ");
		printExpr(statement->statement.expression);
	}
}

void printExpr(ASTNode * expr)
{
	if (expr->type == EXPRESSION) {
		printLogicalOrExpr(expr->expression.precedenceExp);
	}
	else if (expr->type == ASSIGN_EXPRESSION) {
		printLogicalOrExpr(expr->expression.precedenceExp);
		printf(" INT<%s> %s ", expr->expression.id, expr->expression.op);
		printExpr(expr->expression.expression);
	}
	else if (expr->type == COMPOUND_ASSIGN_EXPRESSION) {
		printLogicalOrExpr(expr->expression.precedenceExp);
		printExpr(expr->expression.expression);
	}
}

void printLogicalOrExpr(ASTNode * logicalOrExp)
{
	if (logicalOrExp->type == EXPRESSION) {
		printLogicalAndExpr(logicalOrExp->logicalOrExp.logicalAndExp);
	}
	else if (logicalOrExp->type == BINARY_OP) {
		printLogicalAndExpr(logicalOrExp->logicalOrExp.logicalAndExp);
		printf(" %s ", logicalOrExp->logicalOrExp.op);
		printLogicalOrExpr(logicalOrExp->logicalOrExp.rightExp);
	}

}

void printLogicalAndExpr(ASTNode * logAndExpr)
{
	if (logAndExpr->type == EXPRESSION) {
		printBitwiseOrExpr(logAndExpr->logicalAndExp.bitwiseOrExp);
	}
	else if (logAndExpr->type == BINARY_OP) {
		printBitwiseOrExpr(logAndExpr->logicalAndExp.bitwiseOrExp);
		printf(" %s ", logAndExpr->logicalAndExp.op);
		printLogicalAndExpr(logAndExpr->logicalAndExp.rightLogicalAndExp);
	}
}

void printBitwiseOrExpr(ASTNode * bitwiseOrExpr)
{
	if (bitwiseOrExpr->type == EXPRESSION) {
		printBitwiseXorExpr(bitwiseOrExpr->bitwiseOrExp.bitwiseXorExp);
	}
	else if (bitwiseOrExpr->type == BINARY_OP) {
		printBitwiseXorExpr(bitwiseOrExpr->bitwiseOrExp.bitwiseXorExp);
		printf(" %c ", bitwiseOrExpr->bitwiseOrExp.op);
		printBitwiseOrExpr(bitwiseOrExpr->bitwiseOrExp.rightBitwiseOrExp);
	}
}

void printBitwiseXorExpr(ASTNode * bitwiseXorExpr)
{
	if (bitwiseXorExpr->type == EXPRESSION) {
		printBitwiseAndExpr(bitwiseXorExpr->bitwiseXorExp.bitwiseAndExp);
	}
	else if (bitwiseXorExpr->type == BINARY_OP) {
		printBitwiseAndExpr(bitwiseXorExpr->bitwiseXorExp.bitwiseAndExp);
		printf(" %c ", bitwiseXorExpr->bitwiseXorExp.op);
		printBitwiseXorExpr(bitwiseXorExpr->bitwiseXorExp.rightBitwiseXorExp);
	}
}

void printBitwiseAndExpr(ASTNode * bitwiseAndExpr)
{
	if (bitwiseAndExpr->type == EXPRESSION) {
		printEqualityExpr(bitwiseAndExpr->bitwiseAndExp.equalityExp);
	}
	else if (bitwiseAndExpr->type == BINARY_OP) {
		printEqualityExpr(bitwiseAndExpr->bitwiseAndExp.equalityExp);
		printf(" %c ", bitwiseAndExpr->bitwiseAndExp.op);
		printBitwiseAndExpr(bitwiseAndExpr->bitwiseAndExp.rightBitwiseAndExp);
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

void printRelationalExpr(ASTNode * relaExpr)
{
	if (relaExpr->type == EXPRESSION) {
		printBitwiseShiftExpr(relaExpr->relationalExp.bitwiseShiftExp);
	}
	else if (relaExpr->type == BINARY_OP) {
		printBitwiseShiftExpr(relaExpr->relationalExp.bitwiseShiftExp);
		printf(" %s ", relaExpr->relationalExp.op);
		printRelationalExpr(relaExpr->relationalExp.rightRelationalExp);
	}
}

void printBitwiseShiftExpr(ASTNode * bitwiseShiftExpr)
{
	if (bitwiseShiftExpr->type == EXPRESSION) {
		printAdditiveExpr(bitwiseShiftExpr->bitwiseShiftExp.additiveExp);
	}
	else if (bitwiseShiftExpr->type == BINARY_OP) {
		printAdditiveExpr(bitwiseShiftExpr->bitwiseShiftExp.additiveExp);
		printf(" %s ", bitwiseShiftExpr->bitwiseShiftExp.op);
		printBitwiseShiftExpr(bitwiseShiftExpr->bitwiseShiftExp.rightBitwiseShiftExp);
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
	else if (factor->type == EXPRESSION) {
		printf("(");
		printLogicalOrExpr(factor->factor.expr);
		printf(")");
	}
} 