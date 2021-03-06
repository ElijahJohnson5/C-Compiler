#ifndef AST_H
#define AST_H
typedef enum Type {
	INT
} Type;

const char *getReturnType(Type r);

/*
Crazy ASTNode struct,
contains a union of all the types an Abstract syntax node can be along
with an enum of the type it currently is so you can accsess the correct union
*/
typedef struct ASTNode {
	enum { PROGRAM, FUNCTION, STATEMENT, EXPRESSION, TERM, FACTOR, 
		UNARY_OPERATOR, CONSTANT_INT, BINARY_OP, DECLARATION, BLOCK_ITEM,
		RETURN_STATEMENT, ASSIGN_EXPRESSION, COMPOUND_ASSIGN_EXPRESSION, VARIABLE, IF_STATEMENT } type;
	union {
		//Probabaly move value into factor type
		int value;
		struct {
			//<factor> ::= "(" <exp> ")" | <unary_op> <factor> | <int> | <id>
			union {
				char *id;
				struct ASTNode *expr;
				//<unary_op> ::= "!" | "~" | "-"
				struct {
					char unaryOp;
					struct ASTNode *factor;
				};
			};
		} factor;
		/*
		Handle precedence with differnt structs for each level of precedence
		*/
		struct {
			//Multiplication or divison or modulus
			//<term> ::= <factor> { ("*" | "/" | "%" )  <factor> }
			struct ASTNode *factor;
			struct {
				struct ASTNode *rightTerm;
				char op;
			};

		} term;
		struct {
			//Addition or subtraction
			//<additive-exp> ::= <term> { ("+" | "-")  <term> }
			struct ASTNode *term;
			struct {
				struct ASTNode *rightAdditiveExp;
				char op;
			};
		} additiveExp;
		struct {
			//Bitwise left and right shift
			//<bitwise-shift-exp> ::= <additive-exp> { ("<<" | ">>")  <additive-exp> }
			struct ASTNode *additiveExp;
			struct {
				struct ASTNode *rightBitwiseShiftExp;
				char op[3];
			};
		} bitwiseShiftExp;
		struct {
			//Greater than less than
			//<relational-exp> ::= <bitwise-shift-exp> { ("<" | ">" | "<=" | ">=" )  <bitwise-shift-exp> }
			struct ASTNode *bitwiseShiftExp;
			struct {
				struct ASTNode *rightRelationalExp;
				char op[3];
			};
		} relationalExp;
		struct {
			//equal too or not equal too
			//<equality-exp> ::= <relational-exp> { ("==" | "!=")  <relational-exp> }
			struct ASTNode *relationalExp;
			struct {
				struct ASTNode *rightEqualityExp;
				char op[3];
			};
		} equalityExp;
		struct {
			//Bitwise AND
			//<bitwise-and-exp> ::= <equality-exp> { "&" <equality-exp> }
			struct ASTNode *equalityExp;
			struct {
				struct ASTNode *rightBitwiseAndExp;
				char op;
			};
		} bitwiseAndExp;
		struct {
			//Bitwise XOR
			//<bitwise-xor-exp> ::= <bitwise-and-exp> { "^" <bitwise-and-exp> }
			struct ASTNode *bitwiseAndExp;
			struct {
				struct ASTNode *rightBitwiseXorExp;
				char op;
			};
		} bitwiseXorExp;
		struct {
			//Bitwise OR
			//<bitwise-or-exp> ::= <bitwise-xor-exp> { "|" <bitwise-xor-exp> }
			struct ASTNode *bitwiseXorExp;
			struct {
				struct ASTNode *rightBitwiseOrExp;
				char op;
			};
		} bitwiseOrExp;
		struct {
			//Logical AND
			//<logical-and-exp> ::= <bitwise-or-exp> { "&&" <bitwise-or-exp> }
			struct ASTNode *bitwiseOrExp;
			struct {
				struct ASTNode *rightLogicalAndExp;
				char op[3];
			};
		} logicalAndExp;
		struct {
			//Logical OR
			//<logical-or-exp> ::= <logical-and-exp> { "||" <logical-and-exp> }
			struct ASTNode *logicalAndExp;
			struct {
				struct ASTNode *rightExp;
				char op[3];
			};
		} logicalOrExp;
		struct {
			//<exp> ::= <id> "=" <exp> | <precedence - exp>
			union {
				struct ASTNode *precedenceExp;
				struct {
					char *id;
					char op[4];
					struct ASTNode *expression;
				};
			};
		} expression;
		struct {
			//<program> ::= <function>
			int count;
			struct ASTNode *children;
		} program;
		struct {
			//Zero or more statements
			//Only support int right now
			//<function> ::= "int" <id> "(" ")" "{" { <block-item> } "}"
			Type returnType;
			char *name;
			int blockItemCount;
			struct ASTNode **body;
		} function;
		struct {
			union {
				struct ASTNode *statement;
				struct ASTNode *declaration;
			};
		} blockItem;
		struct {
			//<declaration> ::= "int" <id> [ = <exp>] ";"
			char *id;
			struct ASTNode *declaration;
			struct ASTNode *optionalAssignExpression;
		} declaration;
		struct {
			//<statement> ::= "return" <exp> ";"
			//				| <exp> ";"
			//				| "if" "(" <exp> ")" <statement> [ "else" <statement> ]
			union {
				struct ASTNode *returnExpression;
				struct ASTNode *expression;
				struct {
					struct ASTNode *exp;
					struct ASTNode *statement;
					struct ASTNode *optionalElse;
				};
			};
		} statement;
	};
}ASTNode;

//Free the abstract syntax tree
//Program, function and statement
void freeAST(ASTNode *root);
void freeFunctionAST(ASTNode *function);
void freeStatementAST(ASTNode *statement);
//Expressions
void freeExprAST(ASTNode *expr);
void freeLogicalOrExprAST(ASTNode *logOrExpr);
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