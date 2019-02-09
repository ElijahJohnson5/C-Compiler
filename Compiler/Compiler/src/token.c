#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//Global keywords we support
char *KEYWORDS[] = { "int", "return" };

void freeTokenList(TokenList * list)
{
	//Free list recursivly
	if (list) {
		freeTokenList(list->next);
		freeToken(list->token);
		free(list);
	}
}

void freeToken(Token token)
{
	switch (token.type) {
	case KEYWORD:
		free(token.value.keyword);
		break;
	case IDENTIFIER:
		free(token.value.identifier);
		break;
	}
}

//Create new node and set next to null
TokenList * createTokenList()
{
	TokenList *newToken = malloc(sizeof(TokenList));
	newToken->next = NULL;
	return newToken;
}

//Prints out the token list based on the type
void printTokenList(TokenList * list)
{
	TokenList* temp = list;
	while (temp) {
		printf("{%s}\n", getTokenType(temp->token.type));
		temp = temp->next;
	}
}

//Able to get string from a token type
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
	case ADDITION: return "+";
	case MULTIPLICATION: return "*";
	case DIVISION: return "/";
	case MODULUS: return "%";
	case BITWISE_AND: return "&";
	case BITWISE_OR: return "|";
	case BITWISE_XOR: return "^";
	case BITWISE_SHIFT_LEFT: return "<<";
	case BITWISE_SHIFT_RIGHT: return ">>";
	case ASSIGNMENT: return "=";
	}
	return NULL;
}

int twoCharTokenType(TokenType type)
{
	switch (type) {
	case OPEN_PAREN: return 0;
	case CLOSE_PAREN: return 0;
	case OPEN_BRACE: return 0;
	case CLOSE_BRACE: return 0;
	case SEMICOLON: return 0;
	case KEYWORD: return 0;
	case INT_LITERAL: return 0;
	case IDENTIFIER: return 0;
	case MINUS: return 0;
	case BITWISE_COMPLIMENT: return 0;
	case LOGICAL_NEGATION: return 0;
	case LOGICAL_AND: return 1;
	case LOGICAL_OR: return 1;
	case LESS_THAN_EQUAL_TO: return 1;
	case EQUAL_TO: return 1;
	case GREATER_THAN_EQUAL_TO: return 1;
	case NOT_EQUAL_TO: return 1;
	case LESS_THAN: return 0;
	case GREATER_THAN: return 0;
	case ADDITION: return 0;
	case MULTIPLICATION: return 0;
	case DIVISION: return 0;
	case MODULUS: return 0;
	case BITWISE_AND: return 0;
	case BITWISE_OR: return 0;
	case BITWISE_XOR: return 0;
	case BITWISE_SHIFT_LEFT: return 1;
	case BITWISE_SHIFT_RIGHT: return 1;
	case ASSIGNMENT: return 0;
	}
	return 0;
}

//Check if a string is a two char token
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
	else if (!strcmp(toCheck, "<<")) {
		return BITWISE_SHIFT_LEFT;
	}
	else if (!strcmp(toCheck, ">>")) {
		return BITWISE_SHIFT_RIGHT;
	}

	return -1;
}

//Check if the toCheck is a single char token
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
	case '%':
		return MODULUS;
	case '&':
		return BITWISE_AND;
	case '|':
		return BITWISE_OR;
	case '^':
		return BITWISE_XOR;
	case '=':
		return ASSIGNMENT;
	}

	return -1;
}

//Check string against keyword strings
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

//Check if a token is a unary operator
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
