#ifndef PARSER_H
#define PARSER_H

typedef enum ReturnType {
	INT
} ReturnType;

const char *getReturnType(ReturnType r);

typedef struct ASTNode {
	enum {PROGRAM, FUNCTION, STATEMENT, EXPRESSION, TERM, FACTOR, UNARY_OPERATOR, CONSTANT_INT, BINARY_OP } type;
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
			//Multiplication or divison
			struct ASTNode *factor;
			struct {
				struct ASTNode *rightTerm;
				char op;
			};

		} term;
		struct {
			//Addition or subtraction
			struct ASTNode *term;
			struct {
				struct ASTNode *rightAdditiveExp;
				char op;
			};
		} additiveExp;
		struct {
			//Addition or subtraction
			struct ASTNode *additiveExp;
			struct {
				struct ASTNode *rightRelationalExp;
				char op[3];
			};
		} relationalExp;
		struct {
			//Addition or subtraction
			struct ASTNode *relationalExp;
			struct {
				struct ASTNode *rightEqualityExp;
				char op[3];
			};
		} equalityExp;
		struct {
			//Addition or subtraction
			struct ASTNode *equalityExp;
			struct {
				struct ASTNode *rightLogicalAndExp;
				char op[3];
			};
		} logicalAndExp;
		struct {
			struct ASTNode *logicalAndExp;
			struct {
				struct ASTNode *rightExp;
				char op[3];
			};
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
ASTNode *parseEqualityExpr(struct TokenList** tokens);
ASTNode *parseLogicalAndExpr(struct TokenList** tokens);
ASTNode *parseRelationalExpr(struct TokenList** tokens);
ASTNode *parseAdditiveExpr(struct TokenList** tokens);
ASTNode *parseTerm(struct TokenList** tokens);
ASTNode *parseFactor(struct TokenList** tokens);

void printFactor(ASTNode *factor);
void printTerm(ASTNode *term);
//Expressions
void printAdditiveExpr(ASTNode *addExpr);
void printRelationalExpr(ASTNode *relaExpr);
void printEqualityExpr(ASTNode *eqExpr);
void printLogicalAndExpr(ASTNode *logAndExpr);
void printExpr(ASTNode *expr);
//End expression
void printStatement(ASTNode* statement);
void printFunction(ASTNode* function);
void prettyPrintAST(ASTNode *root);

#endif