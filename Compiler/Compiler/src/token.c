#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *KEYWORDS[] = { "int", "return" };

TokenList * createTokenList()
{
	TokenList *newToken = malloc(sizeof(TokenList));
	newToken->next = NULL;
	return newToken;
}

void printTokenList(TokenList * list)
{
	TokenList* temp = list;
	while (temp) {
		printf("{%s}\n", getTokenType(temp->token.type));
		temp = temp->next;
	}
}

const char * getTokenType(TokenType type)
{
	switch (type) {
	case OPEN_PAREN: return "Open paren";
	case CLOSE_PAREN: return "Close paren";
	case OPEN_BRACE: return "Open brace";
	case CLOSE_BRACE: return "Close brace";
	case SEMICOLON: return "Semicolon";
	case KEYWORD: return "Keyword";
	case INT_LITERAL: return "Literal";
	case IDENTIFIER: return "Identifier";
	case MINUS: return "-";
	case BITWISE_COMPLIMENT: return "~";
	case LOGICAL_NEGATION: return "!";
	case LOGICAL_AND: return "&&";
	case LOGICAL_OR: return "||";
	case LESS_THAN_EQUAL_TO: return "<=";
	case EQUAL_TO: return "==";
	case GREATER_THAN_EQUAL_TO: return ">=";
	case NOT_EQUAL_TO: return "!=";
	case LESS_THAN: return "<";
	case GREATER_THAN: return ">";
	}
	return NULL;
}

int isTwoCharToken(char *toCheck) 
{
	if (!strcmp(toCheck, "&&")) {
		return LOGICAL_AND;
	}
	else if (!strcmp(toCheck, "||")) {
		return LOGICAL_OR;
	}
	else if (!strcmp(toCheck, "<=")) {
		return LESS_THAN_EQUAL_TO;
	}
	else if (!strcmp(toCheck, "==")) {
		return EQUAL_TO;
	}
	else if (!strcmp(toCheck, ">=")) {
		return GREATER_THAN_EQUAL_TO;
	}
	else if (!strcmp(toCheck, "!=")) {
		return NOT_EQUAL_TO;
	}

	return -1;
}

int isToken(char * toCheck)
{
	switch (*toCheck) {
	case '(':
		return OPEN_PAREN;
	case '{':
		return OPEN_BRACE;
	case ')':
		return CLOSE_PAREN;
	case '}':
		return CLOSE_BRACE;
	case ';':
		return SEMICOLON;
	case '-':
		return MINUS;
	case '~':
		return BITWISE_COMPLIMENT;
	case '!':
		return LOGICAL_NEGATION;
	case '+':
		return ADDITION;
	case '*':
		return MULTIPLICATION;
	case '/':
		return DIVISION;
	case '<' :
		return LESS_THAN;
	case '>':
		return GREATER_THAN;
	}

	return -1;
}

char * isKeyword(char * toCheck)
{
	for (int i = 0; i < 2; i++) {
		if (!strcmp(toCheck, KEYWORDS[i])) {
			return toCheck;
		}
	}
	return NULL;
}

int isLiteral(char * toCheck)
{	
	char *s = toCheck;
	while (*s) {
		if (isdigit(*s++) == 0) return -1;
	}
	return atoi(toCheck);
}

int isUnOp(Token next)
{
	switch (next.type) {
	case BITWISE_COMPLIMENT:
	case LOGICAL_NEGATION:
	case MINUS:
		return 1;
	default:
		return 0;
	}
}
