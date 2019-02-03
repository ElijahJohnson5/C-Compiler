#ifndef AST_H
#define AST_H
typedef enum ReturnType {
	INT
} ReturnType;

const char *getReturnType(ReturnType r);

/*
Crazy ASTNode struct,
contains a union of all the types an Abstract syntax node can be along
with an enum of the type it currently is so you can accsess the correct union
*/
typedef struct ASTNode {
	enum { PROGRAM, FUNCTION, STATEMENT, EXPRESSION, TERM, FACTOR, 
		UNARY_OPERATOR, CONSTANT_INT, BINARY_OP, DECLARE_STATEMENT, 
		RETURN_STATEMENT, ASSIGN_EXPRESSION, VARIABLE } type;
	union {
		//Probabaly move value into factor type
		int value;
		struct {
			//<factor> ::= "(" <exp> ")" | <unary_op> <factor> | <int> | <id>
			union {
				char *id;
				struct ASTNode *precedenceExpr;
				//<unary_op> ::= "!" | "~" | "-"
				struct {
					char unaryOp;
					struct ASTNode *factor;
				};
			};
		} factor;
		struct {
			struct ASTNode *exp;
			struct {
				struct ASTNode *rightExp;
				char op[3];
			};
		} precedanceExp;
		struct {
			//<exp> ::= <id> "=" <exp> | <logical-or-exp>
			union {
				struct ASTNode *precedenceExp;
				struct {
					char *id;
					char op;
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
			//<function> ::= "int" <id> "(" ")" "{" { <statement> } "}"
			ReturnType returnType;
			char *name;
			int statementCount;
			struct ASTNode **body;
		} function;
		struct {
			//<statement> ::= "return" <exp> ";"
			//				| <exp> ";"
			//				| "int" <id> [ = <exp>] ";"
			union {
				struct ASTNode *returnExpression;
				struct ASTNode *expression;
				struct {
					//Can be NULL
					struct ASTNode *optinalAssignExpression;
					char *id;
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
void freePrecedenceExprAST(ASTNode *precedenceExpr);
void freeFactorAST(ASTNode *factor);

#endif // !AST_H