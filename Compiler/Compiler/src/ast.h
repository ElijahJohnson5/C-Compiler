#ifndef AST_H
#define AST_H
typedef enum ReturnType {
	INT
} ReturnType;

const char *getReturnType(ReturnType r);

typedef struct ASTNode {
	enum { PROGRAM, FUNCTION, STATEMENT, EXPRESSION, TERM, FACTOR, UNARY_OPERATOR, CONSTANT_INT, BINARY_OP } type;
	union {
		int value;
		struct {
			union {
				struct ASTNode *expression;
				struct {
					char unaryOp;
					struct ASTNode *factor;
				};
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

void freeAST(ASTNode *root);
void freeFunctionAST(ASTNode *function);
void freeStatementAST(ASTNode *statement);
void freeExprAST(ASTNode *expr);
void freeLogicalAndAST(ASTNode *logAndExpr);
void freeEqualityAST(ASTNode *eqExpr);
void freeRelationAST(ASTNode *relaExpr);
void freeAdditiveAST(ASTNode *addExpr);
void freeTermAST(ASTNode *term);
void freeFactorAST(ASTNode *factor);

#endif // !AST_H