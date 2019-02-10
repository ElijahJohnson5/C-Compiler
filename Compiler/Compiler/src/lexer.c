#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "token.h"
#include "lexer.h"

/**
* Read file to string without spaces
*/
char * readFileToString(FILE * f)
{
	char *string;
	long size;
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);

	string = calloc(size + 1, sizeof(char));
	fread(string, size, 1, f);
	string[size] = 0;
	removeNewLinesTabs(string);
	return string;
}

TokenList * tokenizeFile(char * fileString, int *tokenCount)
{
	TokenList *head = createTokenList();
	TokenList *ret = head;
	int tokenType;
	char *value;
	int i = 0;
	*tokenCount = 0;
	//Get all tokens from the file
	while (*fileString) {
		head->token = getNextToken(&fileString);
		(*tokenCount)++;
		if (*fileString) {
			head->next = createTokenList();
			head = head->next;
		}
	}
	return ret;
}

//Remove newlines and tabs from a string
//need spaces for some context
void removeNewLinesTabs(char * source)
{
	char* i = source;
	char* j = source;
	while (*j != 0)
	{
		*i = *j++;
		if (!(*i == '\n' || *i == '\t' || *i == '\r')) {
			i++;
		}
	}
	*i = 0;
}

Token getNextToken(char ** fileString)
{
	Token token;
	int i = 0;
	char current[512];
	//These are tokesn that wont have a space after them
	if (**fileString == ';' || **fileString == '{' || **fileString == '}' || **fileString == '(' || **fileString == ')') {
		current[i++] = **fileString;
		(*fileString)++;
		if (**fileString == ' ') {
			(*fileString)++;
		}
		current[i] = 0;
		token.type = getTokenTypeFromString(current);
		token.value.token[0] = *current;
		token.value.token[1] = 0;
		return token;
	}
	//Loop until we hit a space or one of the tokens above
	while (**fileString != ' ' && **fileString != ';' && **fileString != '{' && **fileString != '}' && **fileString != '(' && **fileString != ')') {
		//Check if we have a token or multiple char token
		if (isToken(current) != -1) {
			break;
		}

		if (isMoreThanOneCharToken(current) != -1) {
			break;
		}
		current[i++] = **fileString;
		(*fileString)++;
	}
	if (**fileString == ' ') {
		(*fileString)++;
	}
	current[i] = 0;
	token.type = getTokenTypeFromString(current);
	switch (token.type) {
		//Set the correct token value
	case KEYWORD:
		token.value.keyword = malloc((strlen(current) + 1) * sizeof(char));
		strncpy(token.value.keyword, current, strlen(current) + 1);
		break;
	case IDENTIFIER:
		token.value.identifier = malloc((strlen(current) + 1) * sizeof(char));
		strncpy(token.value.identifier, current, strlen(current) + 1);
		break;
	case INT_LITERAL:
		token.value.intLiteral = isLiteral(current);
		break;
	default:
		for (int j = 0; j < i; j++) {
			token.value.token[j] = current[j];
		}
		break;
	}
	//Return the next token
	return token;
}
