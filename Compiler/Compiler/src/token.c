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
	}
	return NULL;
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
