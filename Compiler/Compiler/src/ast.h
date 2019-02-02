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
			//Bitwise left and right shift
			struct ASTNode *additiveExp;
			struct {
				struct ASTNode *rightBitwiseShiftExp;
				char op[3];
			};
		} bitwiseShiftExp;
		struct {
			//Greater than less than
			struct ASTNode *bitwiseShiftExp;
			struct {
				struct ASTNode *rightRelationalExp;
				char op[3];
			};
		} relationalExp;
		struct {
			 //equal too or not equal too
			struct ASTNode *relationalExp;
			struct {
				struct ASTNode *rightEqualityExp;
				char op[3];
			};
		} equalityExp;
		struct {
			//Bitwise AND
			struct ASTNode *equalityExp;
			struct {
				struct ASTNode *rightBitwiseAndExp;
				char op;
			};
		} bitwiseAndExp;
		struct {
			//Bitwise XOR
			struct ASTNode *bitwiseAndExp;
			struct {
				struct ASTNode *rightBitwiseXorExp;
				char op;
			};
		} bitwiseXorExp;
		struct {
			//Bitwise OR
			struct ASTNode *bitwiseXorExp;
			struct {
				struct ASTNode *rightBitwiseOrExp;
				char op;
			};
		} bitwiseOrExp;
		struct {
			//Logical AND
			struct ASTNode *bitwiseOrExp;
			struct {
				struct ASTNode *rightLogicalAndExp;
				char op[3];
			};
		} logicalAndExp;
		struct {
			//Logical OR
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

//Program, function and statement
void freeAST(ASTNode *root);
void freeFunctionAST(ASTNode *function);
void freeStatementAST(ASTNode *statement);
//Expressions
void freeExprAST(ASTNode *expr);
void freeLogicalAndAST(ASTNode *logAndExpr);
void freeBitwiseOrAST(ASTNode *bitwiseOrExpr);
void freeBitwiseXorAST(ASTNode *bitwiseXorExpr);
void freeBitwiseAndAST(ASTNode *bitwiseAndExpr);
void freeEqualityAST(ASTNode *eqExpr);
void freeRelationAST(ASTNode *relaExpr);
void freeBitwiseShiftAST(ASTNode *bitwiseShiftExpr);
void freeAdditiveAST(ASTNode *addExpr);
//End expressions

void freeTermAST(ASTNode *term);
void freeFactorAST(ASTNode *factor);

#endif // !AST_H