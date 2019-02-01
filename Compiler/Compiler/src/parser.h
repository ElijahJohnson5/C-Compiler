#ifndef PARSER_H
#define PARSER_H

typedef enum ReturnType {
	INT
} ReturnType;

const char *getReturnType(ReturnType r);

typedef struct ASTNode {
	enum {PROGRAM, FUNCTION, STATEMENT, EXPRESSION, TERM, FACTOR, UNARY_OPERATOR, CONSTANT_INT } type;
	union {
		int value;
		struct {
			union {
				struct ASTNode *expression;
				struct {
					char unaryOp;
					struct ASTNode *factor;
				} factor;
			};
		} factor;
		struct {
			//Can be zero or more
			//Mult or division
			char *op;
			int factorCount;
			//Can be one or more
			struct ASTNode **factor;
		} term;
		struct {
			//Can be zero or more
			//Addition or subtraction
			char *op;
			int termCount;
			//Can be one or more
			struct ASTNode **term;
		} expression;
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
		} statement;
	};
}ASTNode;

ASTNode *parseProgram(struct TokenList** tokens);
ASTNode *parseFunction(struct TokenList** tokens);
ASTNode *parseStatement(struct TokenList** tokens);
ASTNode *parseExpr(struct TokenList** tokens);
ASTNode *parseTerm(struct TokenList** tokens);
ASTNode *parseFactor(struct TokenList** tokens);

void printFactor(ASTNode *factor);
void printTerm(ASTNode *term);
void printExpr(ASTNode *expr);
void printStatement(ASTNode* statement);
void printFunction(ASTNode* function);
void prettyPrintAST(ASTNode *root);

#endif