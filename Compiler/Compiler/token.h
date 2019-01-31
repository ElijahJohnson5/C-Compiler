#ifndef TOKEN_H
#define TOKEN_H

typedef union TokenValue {
	int intLiteral;
	char *keyword;
	char *identifier;
} TokenValue;

typedef enum TokenType {
	OPEN_PAREN, CLOSE_PAREN, OPEN_BRACE, CLOSE_BRACE,
	SEMICOLON, KEYWORD, IDENTIFIER, LITERAL
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

const char* getTokenType(TokenType type);

int isToken(char *toCheck);
char *isKeyword(char *toCheck);
int isLiteral(char *toCheck);

#endif