#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "token.h"
#include "lexer.h"

char * readFileToString(FILE * f)
{
	char *string;
	long size;
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);

	string = calloc(size + 1, sizeof(char));
	fread(string, size, 1, f);
	fclose(f);
	string[size] = 0;
	removeSpaces(string);
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
	char *current = calloc(200, sizeof(char));

	while (*fileString) {
		current[i] = *fileString++;
		i++;
		if ((tokenType = isToken(current)) != -1) {
			head->token.type = tokenType;
			head->token.value.token = *current;
			(*tokenCount)++;
			memset(current, 0, strlen(current));
			i = 0;
			if (*fileString) {
				head->next = createTokenList();
				head = head->next;
			}
			continue;
		}

		if ((value = isKeyword(current)) != NULL) {
			head->token.type = KEYWORD;
			head->token.value.keyword = malloc(strlen(value) + 1 * sizeof(char));
			strncpy(head->token.value.keyword, value, strlen(value) + 1);
			(*tokenCount)++;
			memset(current, 0, strlen(current));
			i = 0;
			if (*fileString) {
				head->next = createTokenList();
				head = head->next;
			}
			continue;
		}

		if ((tokenType = isLiteral(current)) != -1 && isToken(fileString) != -1) {
			head->token.type = INT_LITERAL;
			head->token.value.intLiteral = tokenType;
			(*tokenCount)++;
			memset(current, 0, strlen(current));
			i = 0;
			if (*fileString) {
				head->next = createTokenList();
				head = head->next;
			}
			continue;
		}

		if (isToken(fileString) != -1) {
			head->token.type = IDENTIFIER;
			head->token.value.identifier = malloc(strlen(current) + 1 * sizeof(char));
			strncpy(head->token.value.identifier, current, strlen(current) + 1);
			(*tokenCount)++;
			memset(current, 0, strlen(current));
			i = 0;
			if (*fileString) {
				head->next = createTokenList();
				head = head->next;
			}
			continue;
		}
	}

	return ret;
}

void removeSpaces(char * source)
{
	char* i = source;
	char* j = source;
	while (*j != 0)
	{
		*i = *j++;
		if (!isspace(*i)) {
			i++;
		}
	}
	*i = 0;
}
