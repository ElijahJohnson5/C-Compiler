#ifndef TOKEN_H
#define TOKEN_H

//Union, only need one value at a time
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

//TODO Implement >> <<
//Token types we can handle
typedef enum TokenType {
	OPEN_PAREN, CLOSE_PAREN, OPEN_BRACE, CLOSE_BRACE,
	SEMICOLON, KEYWORD, IDENTIFIER, INT_LITERAL, MINUS,
	BITWISE_COMPLIMENT, LOGICAL_NEGATION, ADDITION, MULTIPLICATION, DIVISION,
	LOGICAL_AND, LOGICAL_OR, EQUAL_TO, NOT_EQUAL_TO, LESS_THAN, LESS_THAN_EQUAL_TO,
	GREATER_THAN, GREATER_THAN_EQUAL_TO, BITWISE_OR, BITWISE_XOR, BITWISE_AND, BITWISE_SHIFT_LEFT,
	MODULUS, BITWISE_SHIFT_RIGHT, ASSIGNMENT
} TokenType;

//A Token has a value and a type
typedef struct Token {
	TokenValue value;
	TokenType type;
} Token;

//Singly linked list for tokens
typedef struct TokenList {
	Token token;
	struct TokenList *next;
} TokenList;

void freeTokenList(TokenList * list);
//Create new node for list
TokenList* createTokenList();
void printTokenList(TokenList *list);

const char* getTokenType(TokenType type);

int isTwoCharToken(char *toCheck);
int isToken(char *toCheck);
char *isKeyword(char *toCheck);
int isLiteral(char *toCheck);
int isUnOp(Token next);

#endif