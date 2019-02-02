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
	//Arbitrary sized current buffer
	char *current = calloc(200, sizeof(char));
	char *twoCharToken = calloc(3, sizeof(char));
	twoCharToken[2] = 0;

	while (*fileString) {
		//Build up a string of current chars
		current[i] = *fileString++;
		twoCharToken[0] = current[i];
		twoCharToken[1] = *fileString;
		i++;
		//Check if it is a token
		//If it is add the token to the list and lear the current string
		if ((tokenType = isToken(current)) != -1) {
			int temp;
			if ((temp = isTwoCharToken(twoCharToken)) != -1) {
				head->token.type = temp;
				head->token.value.leftToken = *current;
				head->token.value.rightToken = *fileString++;
				(*tokenCount)++;
				memset(current, 0, strlen(current));
				i = 0;
				if (*fileString) {
					head->next = createTokenList();
					head = head->next;
				}
			}
			else {
				head->token.type = tokenType;
				head->token.value.token = *current;
				(*tokenCount)++;
				memset(current, 0, strlen(current));
				i = 0;
				if (*fileString) {
					head->next = createTokenList();
					head = head->next;
				}
			}
			continue;
		}

		if ((tokenType = isTwoCharToken(twoCharToken)) != -1) {
			head->token.type = tokenType;
			head->token.value.leftToken = *current;
			head->token.value.rightToken = *fileString++;
			(*tokenCount)++;
			memset(current, 0, strlen(current));
			i = 0;
			if (*fileString) {
				head->next = createTokenList();
				head = head->next;
			}
			continue;
		}

		twoCharToken[0] = *fileString;
		twoCharToken[1] = *(fileString + 1);

		//Check if it is a keyword, if it is add it to the list
		//and clear current
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

		//Check if it is a int literal and the next char is a token
		if ((tokenType = isLiteral(current)) != -1 && (isToken(fileString) != -1 || isTwoCharToken(twoCharToken) != -1)) {
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

		//If the next char is a token then we have an identifier
		if (isToken(fileString) != -1 || isTwoCharToken(twoCharToken) != -1) {
			head->token.type = IDENTIFIER;
			//Copy string in
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

//Remove spaces from a string
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
