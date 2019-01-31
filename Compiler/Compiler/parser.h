#ifndef PARSER_H
#define PARSER_H

typedef enum ReturnType {
	INT
} ReturnType;

const char *getReturnType(ReturnType r);

typedef struct ASTNode {
	enum {PROGRAM, FUNCTION, STATEMENT, EXPRESSION } type;
	union {
		int value;
		struct {
			int count;
			struct ASTNode *children;
		} program;
		struct {
			ReturnType returnType;
			char *name;
			struct ASTNode *body;
		} function;
		struct {
			struct ASTNode *expression;
		} returnStatement;
	};
}ASTNode;

ASTNode *parseProgram(struct TokenList** tokens);
ASTNode *parseFunction(struct TokenList** tokens);
ASTNode *parseStatement(struct TokenList** tokens);
ASTNode *parseExpr(struct TokenList** tokens);

void printExpr(ASTNode *expr);
void printStatement(ASTNode* statement);
void printFunction(ASTNode* function);
void prettyPrintAST(ASTNode *root);

#endif