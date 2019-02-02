#ifndef TOKEN_H
#define TOKEN_H

typedef union TokenValue {
	int intLiteral;
	char *keyword;
	char *identifier;
	char token;
	struct {
		char leftToken;
		char rightToken;
	};
} TokenValue;

typedef enum TokenType {
	OPEN_PAREN, CLOSE_PAREN, OPEN_BRACE, CLOSE_BRACE,
	SEMICOLON, KEYWORD, IDENTIFIER, INT_LITERAL, MINUS,
	BITWISE_COMPLIMENT, LOGICAL_NEGATION, ADDITION, MULTIPLICATION, DIVISION,
	LOGICAL_AND, LOGICAL_OR, EQUAL_TO, NOT_EQUAL_TO, LESS_THAN, LESS_THAN_EQUAL_TO,
	GREATER_THAN, GREATER_THAN_EQUAL_TO
} TokenType;

typedef struct Token {
	TokenValue value;
	TokenType type;
} Token;

typedef struct TokenList {
	Token token;
	struct TokenList *next;
} TokenList;

TokenList* createTokenList();
void printTokenList(TokenList *list);

const char* getTokenType(TokenType type);

int isTwoCharToken(char *toCheck);
int isToken(char *toCheck);
char *isKeyword(char *toCheck);
int isLiteral(char *toCheck);
int isUnOp(Token next);

#endif